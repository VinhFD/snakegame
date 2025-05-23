#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>
#include <vector>
#include <list>

// Define the dimensions of the game screen
extern int nScreenWidth;
extern int nScreenHeight;

// Structure to represent a segment of the snake
struct sSnakeSegment {
    int x; // X coordinate
    int y; // Y coordinate
};

// Structure for obstacles
struct sObstacle {
    std::vector<sSnakeSegment> segments;
    bool isMoving = false;      // Whether the obstacle moves
    int moveDirection = 0;      // 0: up, 1: right, 2: down, 3: left
    int moveSpeed = 1;          // Speed of movement
    int moveCounter = 0;        // Counter for movement delay
};

// Game initialization
void initGame();

// Menu functions
int showMainMenu();
int selectLevel();
void showRecords();

// Obstacle functions
std::vector<sObstacle> generateObstacles(int numObstacles);
void updateObstacles(std::vector<sObstacle>& obstacles);

// Score functions
int getHighScore(int level);
void saveHighScore(int level, int score);

// Game logic
int gameLoop(int level, int highscore);

#endif 