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

// Structure to represent an obstacle
struct sObstacle {
    std::vector<sSnakeSegment> segments; // Vector of segments that make up the obstacle
};

// Game initialization
void initGame();

// Menu functions
int showMainMenu();
int selectLevel();
void showRecords();

// Obstacle functions
std::vector<sObstacle> generateObstacles(int numObstacles);

// Score functions
int getHighScore(int level);
void saveHighScore(int level, int score);

// Game logic
int gameLoop(int level, int highscore);

#endif 