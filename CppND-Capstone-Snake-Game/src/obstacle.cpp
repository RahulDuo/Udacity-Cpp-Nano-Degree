#include "obstacle.h"

void Obstacle::MoveContinuously() {
  while (running_) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [this]() { return !paused_ || !running_; });
      if (!running_) break;

      // Update position
      path_index_ = (path_index_ + 1) % square_path_.size();
      position_ = square_path_[path_index_];
    }

    // Do sleep *outside* the lock
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

void Obstacle::Update() {
  path_index_ = (path_index_ + 1) % square_path_.size();  // loop around the U
}

SDL_Point Obstacle::GetPosition() const {
  return square_path_[path_index_];
}

void Obstacle::Pause() {
  std::lock_guard<std::mutex> lock(mutex_);
  paused_ = true;
}

void Obstacle::Resume() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    paused_ = false;
  }
  cv_.notify_one();
}

void Obstacle::Stop() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    running_ = false;
  }
  cv_.notify_all();
}
