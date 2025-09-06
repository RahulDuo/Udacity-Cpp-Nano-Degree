#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"
#include <string>
#include <memory>

template <typename T>
T MaxValue(const T &a, const T &b) {
  return (a > b) ? a : b;
}

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game(); // Destructor to clean up threads
  
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void SetPlayerName(const std::string &name);
  std::string GetPlayerName() const;
  std::string ScoreMessage();
  std::string ScoreMessage(const std::string &player_name);

 private:
  std::string player_name{"Player"};
  Snake snake;
  SDL_Point food;
  std::unique_ptr<Obstacle> moving_obstacle_;
  
// Threading components
  std::thread obstacle_thread_;
  std::mutex obstacle_mutex_;
  std::condition_variable obstacle_cv_;
  std::atomic<bool> obstacle_running_{false};
  std::atomic<bool> game_paused_{false};
  
  void InitObstacle();
  void StartObstacleThread();
  void StopObstacleThread();
  void ObstacleWorker(); // Thread function for moving obstacle
  SDL_Point GetObstaclePositionSafe(); // Thread-safe getter (no const!)
  bool CheckObstacleCollision(int x, int y) const;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  int LoadHighScore();
  void SaveHighScore(int score);
};

#endif