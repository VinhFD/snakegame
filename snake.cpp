#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <vector>
#include <random>
#include <fstream>
#include "snake.h"

// Define the dimensions of the game screen
int nScreenWidth = 100; // Width of the screen
int nScreenHeight = 45; // Height of the screen

// Function to initialize ncurses
void initGame() {
    initscr(); // Start ncurses mode
    start_color(); // Start color support
    keypad(stdscr, TRUE); // Enable special keys (like arrow keys)
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor

    // Initialize color pairs
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // Border color (cyan text on black background)
    init_pair(2, COLOR_GREEN, COLOR_GREEN); // Snake color
    init_pair(3, COLOR_RED, COLOR_RED); // Food color
    init_pair(4, COLOR_WHITE, COLOR_BLACK); // Text color
    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA); // Obstacle color
    init_pair(6, COLOR_CYAN, COLOR_BLACK); // Menu text color
    init_pair(7, COLOR_RED, COLOR_BLACK); // Game over text color
    init_pair(8, COLOR_RED, COLOR_RED); // Game over background color
    init_pair(9, COLOR_RED, COLOR_BLACK); // Game over elements color
}

// Function to handle the game logic
int gameLoop(int level, int highscore) {
    // Set initial speed based on the selected level
    int initialSpeed = (level == 1) ? 200 : (level == 2) ? 100 : 50; // Speed in milliseconds
    int verticalSpeed = initialSpeed; // Keep the same speed for vertical movement

    // Create a screen buffer to represent the game area
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    
    // Generate obstacles based on level
    int numObstacles = (level == 1) ? 3 : (level == 2) ? 5 : 7;
    std::vector<sObstacle> obstacles = generateObstacles(numObstacles);

    // Reset game state
    std::list<sSnakeSegment> snake = {{11, 15}, {10, 15}, {9, 15}, {8, 15}, {7, 15}, {6, 15}, {5, 15}, {4, 15}, {3, 15}, {2, 15}};
    int nFoodX = 30; // X position of the food
    int nFoodY = 15; // Y position of the food
    int nScore = 0; // Player's score
    int nSnakeDirection = 1; // Initial direction (3 = left)
    bool bDead = false; // Flag to check if the snake is dead

    timeout(initialSpeed); // Set initial timeout based on level

    // Draw the initial state of the game
    for (auto s : snake) {
        screen[s.y * nScreenWidth + s.x] = L'@'; // Draw the snake segments
    }
    screen[nFoodY * nScreenWidth + nFoodX] = L'%'; // Draw the food

    // Draw obstacles
    for (const auto& obs : obstacles) {
        for (const auto& seg : obs.segments) {
            screen[seg.y * nScreenWidth + seg.x] = L'#'; // Draw obstacle segments
        }
    }

    // Refresh the screen to show the initial state
    for (int y = 0; y < nScreenHeight; y++) {
        for (int x = 0; x < nScreenWidth; x++) {
            mvaddch(y, x, screen[y * nScreenWidth + x]); // Render the screen buffer
        }
    }
    refresh(); // Refresh the screen to show updates

    while (!bDead) { // Game loop continues until the snake is dead
        auto t1 = std::chrono::system_clock::now(); // Start timing for movement

        // Update obstacle positions
        if (level >= 2) { // Only move obstacles in hard and super hard modes
            updateObstacles(obstacles);
        }

        // Handle user input for snake movement
        while ((std::chrono::system_clock::now() - t1) < std::chrono::milliseconds(initialSpeed)) {
            int ch = getch(); // Get user input
            switch (ch) {
                case 'q': // Quit the game
                    endwin(); // End ncurses mode
                    return nScore; // Exit the game
                case KEY_LEFT: // Move left
                    if (nSnakeDirection != 1) nSnakeDirection = 3; // Change direction to left
                    break;
                case KEY_RIGHT: // Move right
                    if (nSnakeDirection != 3) nSnakeDirection = 1; // Change direction to right
                    break;
                case KEY_UP: // Move up
                    if (nSnakeDirection != 2) nSnakeDirection = 0; // Change direction to up
                    break;
                case KEY_DOWN: // Move down
                    if (nSnakeDirection != 0) nSnakeDirection = 2; // Change direction to down
                    break;
            }

            // Move the snake based on the current direction
            switch (nSnakeDirection) {
                case 0: // Move up
                    snake.push_front({snake.front().x, snake.front().y - 1});
                    break;
                case 1: // Move right
                    snake.push_front({snake.front().x + 1, snake.front().y});
                    break;
                case 2: // Move down
                    snake.push_front({snake.front().x, snake.front().y + 1});
                    break;
                case 3: // Move left
                    snake.push_front({snake.front().x - 1, snake.front().y});
                    break;
                default: break;
            }

            // Check for food collision
            if (snake.front().x == nFoodX && snake.front().y == nFoodY) {
                nScore++; // Increase score
                initialSpeed = std::max(10, initialSpeed - (level == 1 ? 20 : (level == 2 ? 15 : 10))); // Decrease speed but not below 10ms
                timeout(initialSpeed); // Update timeout based on new speed
                
                // Randomly place food in an empty space
                bool validPosition = false;
                while (!validPosition) {
                    nFoodX = rand() % (nScreenWidth - 2) + 1; // Random X position (avoid borders)
                    nFoodY = (rand() % (nScreenHeight - 4)) + 3; // Random Y position (avoid borders and score area)
                    
                    // Check if position is empty
                    validPosition = true;
                    
                    // Check if food is on snake
                    for (const auto& segment : snake) {
                        if (segment.x == nFoodX && segment.y == nFoodY) {
                            validPosition = false;
                            break;
                        }
                    }
                    
                    // Check if food is on any obstacle
                    if (validPosition) {
                        for (const auto& obs : obstacles) {
                            for (const auto& seg : obs.segments) {
                                if (seg.x == nFoodX && seg.y == nFoodY) {
                                    validPosition = false;
                                    break;
                                }
                            }
                            if (!validPosition) break;
                        }
                    }
                }
                
                for (int i = 0; i < 5; i++) {
                    snake.push_back({snake.back().x, snake.back().y}); // Grow the snake
                }
            }

            // Check for wall collision
            if (snake.front().x < 0) {
                snake.front().x = nScreenWidth - 1; // Wrap to the right side
            } else if (snake.front().x >= nScreenWidth) {
                snake.front().x = 0; // Wrap to the left side
            }

            if (snake.front().y < 3) {
                snake.front().y = nScreenHeight - 1; // Wrap to the bottom
            } else if (snake.front().y >= nScreenHeight) {
                snake.front().y = 3; // Wrap to the top
            }

            // Check for obstacle collision
            for (const auto& obs : obstacles) {
                for (const auto& seg : obs.segments) {
                    // Check if snake head collides with any obstacle segment
                    if (snake.front().x == seg.x && snake.front().y == seg.y) {
                        bDead = true; // Snake hits obstacle
                        break;
                    }
                    // Check if any snake segment collides with any obstacle segment
                    for (auto it = snake.begin(); it != snake.end(); ++it) {
                        if (it->x == seg.x && it->y == seg.y) {
                            bDead = true; // Snake segment hits obstacle
                            break;
                        }
                    }
                    if (bDead) break;
                }
                if (bDead) break;
            }

            // Check for self-collision
            for (auto i = snake.begin(); i != snake.end(); i++) {
                if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y) {
                    bDead = true; // Snake hits itself
                }
            }

            snake.pop_back(); // Remove the last segment of the snake

            // Clear the screen
            for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
                screen[i] = L' '; // Reset the screen buffer
            }

            // Draw the borders
            for (int i = 0; i < nScreenWidth; i++) {
                screen[i] = L'='; // Top border
                screen[2 * nScreenWidth + i] = L'='; // Score area bottom border
                screen[(nScreenHeight-1) * nScreenWidth + i] = L'='; // Bottom border
            }
            for (int y = 0; y < nScreenHeight; y++) {
                screen[y * nScreenWidth] = L'|'; // Left border
                screen[y * nScreenWidth + nScreenWidth - 1] = L'|'; // Right border
            }

            // Draw the score
            std::string scoreStr = "Snake Game   SCORE: " + std::to_string(nScore);
            for (size_t i = 0; i < scoreStr.size() && i < nScreenWidth - 2; i++) {
                screen[1 * nScreenWidth + 5 + i] = scoreStr[i]; // Display score on the screen
            }

            // Draw obstacles
            for (const auto& obs : obstacles) {
                for (const auto& seg : obs.segments) {
                    if (seg.x > 0 && seg.x < nScreenWidth - 1 && seg.y > 2 && seg.y < nScreenHeight - 1) {
                        screen[seg.y * nScreenWidth + seg.x] = L'#'; // Draw obstacle segments
                    }
                }
            }

            // Draw the snake
            for (auto s : snake) {
                if (s.x > 0 && s.x < nScreenWidth - 1 && s.y > 2 && s.y < nScreenHeight - 1) {
                    screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'@'; // Draw snake segments
                }
            }
            if (snake.front().x > 0 && snake.front().x < nScreenWidth - 1 && 
                snake.front().y > 2 && snake.front().y < nScreenHeight - 1) {
                screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@'; // Draw snake head
            }

            // Draw food
            if (nFoodX > 0 && nFoodX < nScreenWidth - 1 && nFoodY > 2 && nFoodY < nScreenHeight - 1) {
                screen[nFoodY * nScreenWidth + nFoodX] = L'%'; // Draw food
            }

            // Refresh the screen
            for (int y = 0; y < nScreenHeight; y++) {
                for (int x = 0; x < nScreenWidth; x++) {
                    // Only draw if within screen bounds
                    if (x < nScreenWidth && y < nScreenHeight) {
                        // Use color pairs for drawing
                        if (y == 0 || y == 2 || y == nScreenHeight - 1 || x == 0 || x == nScreenWidth - 1) {
                            attron(COLOR_PAIR(1)); // Border color (cyan)
                        } else if (screen[y * nScreenWidth + x] == L'@') {
                            attron(COLOR_PAIR(2)); // Snake color
                        } else if (screen[y * nScreenWidth + x] == L'%') {
                            attron(COLOR_PAIR(3)); // Food color
                        } else if (screen[y * nScreenWidth + x] == L'#') {
                            attron(COLOR_PAIR(5)); // Obstacle color
                        } else {
                            attron(COLOR_PAIR(4)); // Text color
                        }
                        mvaddch(y, x, screen[y * nScreenWidth + x]); // Render the screen buffer
                        attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(5)); // Turn off color attributes
                    }
                }
            }
            refresh(); // Refresh the screen to show updates
        }
        if(bDead){
            // First make all game elements red
            for (int y = 0; y < nScreenHeight; y++) {
                for (int x = 0; x < nScreenWidth; x++) {
                    if (screen[y * nScreenWidth + x] != ' ') {
                        attron(COLOR_PAIR(9)); // Red elements
                        mvaddch(y, x, screen[y * nScreenWidth + x]);
                        attroff(COLOR_PAIR(9));
                    }
                }
            }
            refresh();

            // Then display game over message in red on top
            attron(COLOR_PAIR(7)); // Red text
            std::string gameOverStr = "PRESS 'SPACE' TO PLAY AGAIN";
            for (size_t i = 0; i < gameOverStr.size(); i++) {
                mvaddch(nScreenHeight / 2, (nScreenWidth - gameOverStr.size()) / 2 + i, gameOverStr[i]);
            }
            attroff(COLOR_PAIR(7));
            refresh();

            // Wait for space to restart
            while (true) {
                int restartCh = getch();
                if (restartCh == ' ') {
                    break;
                }
            }
        }
    }

    delete[] screen;
    endwin();
    return nScore;
}

// Main function
// int main()
// {
//     while (true) // Outer loop for main menu
//     {
//         initGame(); // Initialize the game

//         int menuChoice = showMainMenu();
//         if (menuChoice == 1) { // Game option
//             // Level selection
//             int level = selectLevel(); // Get the selected level
//             int highscore = getHighScore(level);
//             int score = gameLoop(level, highscore); // Start the game loop with the selected level
//             if (score > highscore) {
//                 saveHighScore(level, score);
//             }
//         } else { // Record option
//             showRecords();
//         }
//     }
// }