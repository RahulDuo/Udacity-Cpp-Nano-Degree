#include "game.h"
#include <iostream>
#include "SDL.h"
#include <fstream>
#include <string>
#include <chrono>

void Game::InitObstacle() {
  std::vector<SDL_Point> square_path;

  int center_x = 16; // kGridWidth / 2;  
  int center_y = 16; // kGridHeight / 2; 
  int half_side = 4;               // 8x8 square

  int left = center_x - half_side;    // 12
  int right = center_x + half_side;   // 20
  int top = center_y - half_side;     // 12
  int bottom = center_y + half_side;  // 20

  // Top side (left to right)
  for (int x = left; x <= right; ++x) {
    square_path.push_back({x, top});
  }
  // Right side (top to bottom)
  for (int y = top + 1; y <= bottom; ++y) {
    square_path.push_back({right, y});
  }
  // Bottom side (right to left)
  for (int x = right - 1; x >= left; --x) {
    square_path.push_back({x, bottom});
  }
  // Left side (bottom to top)
  for (int y = bottom - 1; y > top; --y) {
    square_path.push_back({left, y});
  }

  moving_obstacle_ = std::make_unique<Obstacle>(square_path);
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
   PlaceFood();
   InitObstacle();
   StartObstacleThread(); // Start the obstacle thread
}

Game::~Game() {
  StopObstacleThread(); // Clean up thread on destruction
}

void Game::StartObstacleThread() {
  obstacle_running_ = true;
  obstacle_thread_ = std::thread(&Game::ObstacleWorker, this);
}

void Game::StopObstacleThread() {
  if (obstacle_running_) {
    obstacle_running_ = false;
    obstacle_cv_.notify_all(); // Wake up the thread to exit
    
    if (obstacle_thread_.joinable()) {
      obstacle_thread_.join();
    }
  }
}

void Game::ObstacleWorker() {
  std::unique_lock<std::mutex> lock(obstacle_mutex_);
  
  while (obstacle_running_) {
    // Wait for 200ms or until notified to stop
    if (obstacle_cv_.wait_for(lock, std::chrono::milliseconds(200)) == std::cv_status::timeout) {
      // Only update if game is not paused and still running
      if (obstacle_running_ && !game_paused_ && moving_obstacle_) {
        moving_obstacle_->Update();
//          std::cout << "Obstacle updated to position: " << moving_obstacle_->GetPosition().x 
//                   << ", " << moving_obstacle_->GetPosition().y << std::endl; // Debug output
      }
    }
  }
}

SDL_Point Game::GetObstaclePositionSafe() {
  std::lock_guard<std::mutex> lock(obstacle_mutex_);
  return moving_obstacle_ ? moving_obstacle_->GetPosition() : SDL_Point{-1, -1};
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool paused = false;

  int high_score = LoadHighScore();

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, paused);
    
    // Update pause state for obstacle thread
    game_paused_ = paused;

    // Pausing logic
    if (!paused) {
      // Snake movement and game logic
      Update();

      // Check for collision with the moving obstacle (thread-safe)
      SDL_Point head{ 
        static_cast<int>(snake.GetHeadX()), 
        static_cast<int>(snake.GetHeadY()) 
      };
      
      SDL_Point obs = GetObstaclePositionSafe(); // Thread-safe access
      if (obs.x != -1 && obs.y != -1 && head.x == obs.x && head.y == obs.y) {
        running = false;
        continue;  // skip rendering the frame where you died
      }
    }

    // Render everything (food, snake, and obstacle)
    if (moving_obstacle_) {
      // Thread-safe access to obstacle for rendering
      std::lock_guard<std::mutex> lock(obstacle_mutex_);
      renderer.Render(snake, food, moving_obstacle_.get());	
    } else {
      renderer.Render(snake, food, nullptr);
    }
  
    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;

    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count, high_score);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  // Game over: update high score 
  int final_score = MaxValue(score, high_score);
  SaveHighScore(final_score);

  return;
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int new_x = static_cast<int>(snake.GetHeadX());
  int new_y = static_cast<int>(snake.GetHeadY());

  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    snake.GrowBody();

    float current_speed = snake.GetSpeed();
    snake.SetSpeed(current_speed + 0.02f);
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }

int Game::LoadHighScore() {
  std::ifstream file("../highscore.txt");
  int score = 0;

  if (file.is_open()) {
    std::string line;
    if (std::getline(file, line)) {
      try {
        score = std::stoi(line);
        if (score < 0) {
          score = 0;
        }
      } catch (const std::invalid_argument &e) {
        std::cerr << "[Warning] Invalid content in highscore.txt: not a number.\n";
      } catch (const std::out_of_range &e) {
        std::cerr << "[Warning] Number in highscore.txt out of range.\n";
      }
    }
    file.close();
  }

  return score;
}

void Game::SaveHighScore(int score) {
  std::ofstream file("../highscore.txt");
  if (file.is_open()) {
    file << score;
    file.close();
  }
}

std::string Game::ScoreMessage() {
  return "Score: " + std::to_string(score);
}

std::string Game::ScoreMessage(const std::string &player_name) {
  return player_name + "'s Score: " + std::to_string(score);
}

void Game::SetPlayerName(const std::string &name) {
    player_name = name;
}

std::string Game::GetPlayerName() const {
    return player_name;
}