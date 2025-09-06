#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>

class Obstacle {
 public:
  Obstacle() = default;
 Obstacle(std::vector<SDL_Point> const &path)
      : square_path_(path), path_index_(0) {}

  void Init();
  
  void MoveContinuously();
  SDL_Point GetPosition() const;

  void Pause();
  void Resume();
  void Stop();
  void Update();

 private:
  std::vector<SDL_Point> square_path_;
  SDL_Point position_;
  size_t path_index_{0};

  mutable std::mutex mutex_;
  std::condition_variable cv_;
  bool paused_{false};
  bool running_{true};
};
#endif // OBSTACLE_H
