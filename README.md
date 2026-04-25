# 🐍 Snake Game

A classic Snake game implemented in C++ for the Windows console. Navigate the snake to eat food, grow longer, and rack up your high score — without hitting the walls!

---

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Building & Running](#building--running)
- [How to Play](#how-to-play)
- [Scoring](#scoring)
- [Project Structure](#project-structure)
- [Code Architecture](#code-architecture)
- [Known Limitations](#known-limitations)
- [Potential Improvements](#potential-improvements)

---

## Features

- Console-based rendering in an 80×24 character grid
- Real-time keyboard input (arrow keys)
- Snake grows in length each time food is eaten
- Collision detection with walls
- Score tracking
- Centered Game Over screen with final score

---

## Requirements

| Requirement | Details |
|---|---|
| **OS** | Windows (uses `<windows.h>` and `GetAsyncKeyState`) |
| **Compiler** | Any C++11-compatible compiler (MSVC, MinGW/GCC) |
| **Console** | Standard Windows Command Prompt or PowerShell |

> **Note:** This game uses Windows-specific APIs (`GetAsyncKeyState`, `Sleep`, `system("cls")`). It will not compile or run on Linux or macOS without significant modification.

---

## Building & Running

### Using MinGW (g++)

```bash
g++ -o SnakeGame SnakeGame.cpp
SnakeGame.exe
```

### Using MSVC (Developer Command Prompt)

```bash
cl SnakeGame.cpp /Fe:SnakeGame.exe
SnakeGame.exe
```

---

## How to Play

| Key | Action |
|---|---|
| `↑` Arrow | Move Up |
| `↓` Arrow | Move Down |
| `←` Arrow | Move Left |
| `→` Arrow | Move Right |

- The snake starts in the **top-left corner** of the play area, moving right.
- Steer the snake (`O` = head, `#` = body) toward the food (`$`).
- **Eating food** makes the snake grow by one segment and adds 10 points to your score.
- **Hitting a wall** ends the game immediately.
- After a Game Over, press any key to exit.

---

## Scoring

| Event | Points |
|---|---|
| Eating one food item | +10 |

Your current score is displayed **below the game area** each frame and on the Game Over screen.

---

## Project Structure

```
SnakeGame.cpp      # Single-file source containing all game logic
```

The entire game is contained in one `.cpp` file with three main classes and one supporting struct.

---

## Code Architecture

### `SnakePosition` (struct)
A simple pair of `int x` and `int y` coordinates, used by both the snake segments and the food item.

### `Family` (class) — The Game Board
Manages the 80×24 character grid.

- Allocates and frees a 2D `char` array on the heap.
- `clear()` — redraws the border (`#`) and fills the interior with spaces.
- `Draw(y, x, char)` — places a single character inside the safe play area.
- `DrawTextMessage(y, x, string)` — writes a string at the given grid position (used for the Game Over message).
- `print()` — flushes the entire grid to `stdout`.
- A single global instance `F` is used throughout.

### `FoodPosition` (class)
Tracks the location and display symbol (`$`) of the food.

- `reposition(Family&)` — picks a new random location inside the border using `rand()`.
- A single global instance `Food` is used throughout.

### `Snake` (class)
The core game entity. Stores up to 100 body segments in a fixed-size array.

- **Direction** is an anonymous enum: `UP`, `DOWN`, `LEFT`, `RIGHT`.
- `GetInput(Family&)` — polls arrow keys with `GetAsyncKeyState`; a `can_turn` flag prevents the snake from reversing direction mid-frame.
- `Move(Family&)` — shifts each body segment to the position of the one ahead of it, advances the head, then throws a `const char*` exception if the head goes out of bounds.
- `Draw(Family&)` — renders the head as `O` and each body segment as `#`.
- `CheckFood(FoodPosition&)` — returns `true` and increments size/score when the head overlaps the food.
- A single global instance `Snake(1, 1)` is used throughout.

### `main()` — Game Loop

```
Initialize board and food
Loop forever:
    Clear board
    Poll keyboard input
    Move snake  →  catch wall-collision exception → show Game Over, exit
    Draw snake and food
    Check food collision → reposition food if eaten
    Print score and board
    Sleep ~33 ms  (~30 FPS)
    Clear console
```

---

## Known Limitations

- **Windows-only** — relies on `<windows.h>`, `GetAsyncKeyState`, `Sleep`, and `system("cls")`.
- **No self-collision detection** — the snake can pass through its own body without penalty.
- **Fixed 100-segment cap** — the `Position` array is statically sized; a snake longer than 100 segments would cause undefined behaviour.
- **`rand()` is not seeded** — food always spawns in the same sequence every run. Adding `srand(time(nullptr))` would fix this.
- **Global instances** — `F`, `Food`, and `Snake` are global objects, which tightly couples all classes.
- **`system("cls")`** — causes visible screen flicker; double-buffering would produce smoother output.
- **No restart option** — the program exits after Game Over; the player must relaunch to play again.

---

## Potential Improvements

- Seed `rand()` with `srand((unsigned)time(nullptr))` for varied food placement each run.
- Replace the fixed `Position[100]` array with a `std::deque<SnakePosition>` to support unlimited growth.
- Add self-collision detection to end the game when the head hits the body.
- Use double-buffering or the Windows Console API (`WriteConsoleOutputCharacter`) to eliminate screen flicker.
- Refactor global instances into a `Game` class for cleaner encapsulation.
- Add difficulty levels by increasing `Speed` or reducing the `Sleep` duration as the snake grows.
- Add a high-score tracker that persists between sessions using file I/O.
- Provide a restart prompt on Game Over instead of exiting.
