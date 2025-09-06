# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## 🚀 New Features Added

This capstone project extends the original Snake game with the following enhancements:

1. **Player Name Entry**
   - Before launching the game window, the player is prompted to enter their name.
   - After the game ends, a custom score message displays the player’s name and score.
   - This helps personalize the game and sets the foundation for high-score tracking.

2. **Pause & Quit Features**
   - Pressing `P` during gameplay toggles the pause/resume state of the game.
   - Pressing `Q` during gameplay quits the game immediately.

3. **Moving Obstacle**
   - Added a dynamic moving obstacle that moves along a square path centered on the game grid.
   - The obstacle moves step-by-step along the path each frame (with timed updates).
   - Collision with the obstacle ends the game.

4. **High Score Persistence**
   - High scores are read from and saved to `highscore.txt`.
   - The highest score is displayed in the window title bar during gameplay.

---

## ⚙️ Important Build Note

Since new files were added (`obstacle.h` and `obstacle.cpp`), **you need to update your `CMakeLists.txt`** to include these files so they are compiled and linked correctly. For example, add:

```cmake
add_executable(SnakeGame
  src/main.cpp
  src/game.cpp
  src/controller.cpp
  src/renderer.cpp
  src/snake.cpp
  src/obstacle.cpp   # Add this line
)
```
## ✅ Rubric Mapping and Project Criteria Fulfilled
## Rubric Points Addressed

### Loops, Functions, I/O
* **Functions and control structures** (`controller.cpp` lines 15-35, `game.cpp` throughout): Multiple control structures including while loops, for loops, switch statements, and if conditions
* **File I/O** (`game.cpp` lines 196-218): High score loading from and saving to `highscore.txt`
* **User input** (`main.cpp` line 8, `controller.cpp` lines 15-45): Player name input and keyboard controls
* **Data structures** (`snake.h` line 49, `obstacle.h` line 30): Vectors for snake body and obstacle path, const variables throughout

### Object Oriented Programming
* **Classes with access specifiers** (All `.h` files): Game, Snake, Controller, Obstacle, and Renderer classes with proper public/private sections
* **Member initialization lists** (`snake.h` lines 11-15, `game.cpp` lines 36-40): Snake and Game constructors use initialization lists
* **Function overloading** (`game.h` lines 23-24, `game.cpp` lines 220-226): `ScoreMessage()` function overloaded with different signatures
* **Templates** (`game.h` lines 14-17): `MaxValue` template function for generic type comparison
* **Getter/Setter Usage** - `Snake` getters used throughout (`GetSpeed`, `GetHeadX`, `IsAlive`, etc.): `snake.cpp`

### Memory Management
* **References in function declarations** (`controller.cpp` line 6-9, `game.cpp` line 91): Multiple functions use pass-by-reference
* **Destructors** (`game.cpp` lines 50-52): Game destructor properly manages thread cleanup
* **Smart pointers** (`game.h` line 40, `game.cpp` line 37): `std::unique_ptr<Obstacle>` used for obstacle management

### Concurrency
* **Multithreading** (`game.cpp` lines 54-85): Separate thread for obstacle movement using `std::thread`
* **Mutex/locks** (`game.cpp` lines 71, 133, `obstacle.cpp`): `std::mutex` and `std::lock_guard` protect shared data
* **Condition variable** (`game.cpp` lines 75, `obstacle.h` lines 33): `std::condition_variable` synchronizes thread execution







