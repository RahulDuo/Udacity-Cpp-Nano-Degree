#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  
   std::string player_name;
  std::cout << "Enter your name: ";
  std::getline(std::cin, player_name);
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
//   std::cout << "Size: " << game.GetSize() << "\n";
  
   std::cout << game.ScoreMessage(player_name) << "\n";
  return 0;
}