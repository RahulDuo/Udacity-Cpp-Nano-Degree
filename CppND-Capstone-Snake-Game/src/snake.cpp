#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(GetHeadX()),
      static_cast<int>(GetHeadY())
  };  // Capture the head's cell before updating.

  UpdateHead();

  SDL_Point current_cell{
      static_cast<int>(GetHeadX()),
      static_cast<int>(GetHeadY())
  };  // Capture the head's cell after updating.

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= GetSpeed();
      break;
    case Direction::kDown:
      head_y += GetSpeed();
      break;
    case Direction::kLeft:
      head_x -= GetSpeed();
      break;
    case Direction::kRight:
      head_x += GetSpeed();
      break;
  }

  // Wrap around the grid
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  body.push_back(prev_head_cell);

  if (!growing) {
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check for self-collision
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(GetHeadX()) && y == static_cast<int>(GetHeadY())) {
    return true;
  }
  for (auto const &item : GetBody()) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
