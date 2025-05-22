# Snake Game

A classic Snake game implementation in C++ using the ncurses library.

## Features

- Classic snake gameplay
- Multiple difficulty levels
- Score tracking
- High score system
- Colorful interface
- Obstacle system

## Requirements

- C++ compiler (g++ or compatible)
- ncurses library
- Make

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/snake-game.git
cd snake-game
```

2. Compile the game:
```bash
make
```

## How to Play

1. Run the game:
```bash
./snake
```

2. Controls:
- Arrow keys to move the snake
- 'q' to quit the game
- Space to restart after game over

## Game Rules

- Guide the snake to eat the food (%)
- Avoid hitting the walls, obstacles (#), and yourself
- The snake grows longer each time it eats
- The game speeds up as you score more points

## Building from Source

The project uses a Makefile for building. To clean and rebuild:

```bash
make clean
make
```

## License

This project is open source and available under the MIT License. 