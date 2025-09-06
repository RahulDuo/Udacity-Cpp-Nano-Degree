#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(1),  // to start the snake from left corner
        head_y(grid_height / 2) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;
  void SetDirection(Direction dir) { direction = dir; }

  float GetSpeed() const { return speed; }
  void SetSpeed(float s) { speed = s; }

  int GetSize() const { return size; }
  bool IsAlive() const { return alive; }

  float GetHeadX() const { return head_x; }
  float GetHeadY() const { return head_y; }

  const std::vector<SDL_Point>& GetBody() const { return body; }


  

 private:
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif