#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>

// Define the dimensions of the game screen
int nScreenWidth = 100; // Width of the screen
int nScreenHeight = 45; // Height of the screen

// Structure to represent a segment of the snake
struct sSnakeSegment
{
    int x; // X coordinate
    int y; // Y coordinate
};

// Function to initialize ncurses
void initGame() {
    initscr(); // Start ncurses mode
    keypad(stdscr, TRUE); // Enable special keys (like arrow keys)
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor
}

// Function to display the level selection menu
int selectLevel() {
    int level; // Variable to store the selected level
    while (true) {
        clear(); // Clear the screen
        mvprintw(5, 5, "Select Level:"); // Display level selection prompt
        mvprintw(6, 5, "1. Easy"); // Option for Easy level
        mvprintw(7, 5, "2. Hard"); // Option for Hard level
        mvprintw(8, 5, "3. Super Hard"); // Option for Super Hard level
        mvprintw(9, 5, "Enter your choice (1-3): "); // Prompt for user input
        refresh(); // Refresh the screen to show updates

        int ch = getch(); // Get user input
        if (ch >= '1' && ch <= '3') { // Check if input is valid
            level = ch - '0'; // Convert char to int (1, 2, or 3)
            break; // Exit the loop
        }
    }
    return level; // Return the selected level
}

// Function to handle the game logic
void gameLoop(int level) {
    // Set initial speed based on the selected level
    int initialSpeed = (level == 1) ? 200 : (level == 2) ? 100 : 50; // Speed in milliseconds
    int verticalSpeed = initialSpeed * 2; // Half of the horizontal speed

    // Create a screen buffer to represent the game area
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
        screen[i] = L' '; // Initialize the screen with spaces
    }

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

    // Refresh the screen to show the initial state
    for (int y = 0; y < nScreenHeight; y++) {
        for (int x = 0; x < nScreenWidth; x++) {
            mvaddch(y, x, screen[y * nScreenWidth + x]); // Render the screen buffer
        }
    }
    refresh(); // Refresh the screen to show updates

    while (!bDead) { // Game loop continues until the snake is dead
        auto t1 = std::chrono::system_clock::now(); // Start timing for movement

        // Handle user input for snake movement
        while ((std::chrono::system_clock::now() - t1) < std::chrono::milliseconds(initialSpeed)) {
            int ch = getch(); // Get user input
            switch (ch) {
                case 'q': // Quit the game
                    endwin(); // End ncurses mode
                    return; // Exit the game
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
                    timeout(verticalSpeed); // Set timeout for vertical movement
                    break;
                case 1: // Move right
                    snake.push_front({snake.front().x + 1, snake.front().y});
                    timeout(initialSpeed); // Set timeout for horizontal movement
                    break;
                case 2: // Move down
                    snake.push_front({snake.front().x, snake.front().y + 1});
                    timeout(verticalSpeed); // Set timeout for vertical movement
                    break;
                case 3: // Move left
                    snake.push_front({snake.front().x - 1, snake.front().y});
                    timeout(initialSpeed); // Set timeout for horizontal movement
                    break;
                default: break;
            }

            // Check for food collision
            if (snake.front().x == nFoodX && snake.front().y == nFoodY) {
                nScore++; // Increase score
                initialSpeed = std::max(10, initialSpeed - (level == 1 ? 20 : (level == 2 ? 15 : 10))); // Decrease speed but not below 10ms
                timeout(initialSpeed); // Update timeout based on new speed
                // Randomly place food in an empty space
                while (screen[nFoodY * nScreenWidth + nFoodX] != ' ') {
                    nFoodX = rand() % nScreenWidth; // Random X position
                    nFoodY = (rand() % (nScreenHeight - 3)) + 3; // Random Y position
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
                screen[2 * nScreenWidth + i] = L'='; // Bottom border
                screen[(nScreenHeight-1) * nScreenWidth + i] = L'='; // Bottom border
            }
            for (int y = 0; y < nScreenHeight; y++) {
                screen[y * nScreenWidth] = L'|'; // Left border
                screen[y * nScreenWidth + nScreenWidth - 1] = L'|'; // Right border
            }

            // Draw the score
            std::string scoreStr = "Snake Game   SCORE: " + std::to_string(nScore);
            for (size_t i = 0; i < scoreStr.size(); i++) {
                screen[1 * nScreenWidth + 5 + i] = scoreStr[i]; // Display score on the screen
            }

            // Draw the snake
            for (auto s : snake) {
                screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'@'; // Draw snake segments
            }
            screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@'; // Draw snake head
            screen[nFoodY * nScreenWidth + nFoodX] = L'%'; // Draw food

            // Refresh the screen
            for (int y = 0; y < nScreenHeight; y++) {
                for (int x = 0; x < nScreenWidth; x++) {
                    mvaddch(y, x, screen[y * nScreenWidth + x]); // Render the screen buffer
                }
            }
            refresh(); // Refresh the screen to show updates
        }
        if(bDead){
            // Game over logic
            std::string gameOverStr = "PRESS 'SPACE' TO PLAY AGAIN"; // Message to restart the game
            for (size_t i = 0; i < gameOverStr.size(); i++) {
                screen[(nScreenHeight / 2) * nScreenWidth + (nScreenWidth - gameOverStr.size()) / 2 + i] = gameOverStr[i]; // Center the message
            }

            // Refresh the screen to show the game over message
            for (int y = 0; y < nScreenHeight; y++) {
                for (int x = 0; x < nScreenWidth; x++) {
                    mvaddch(y, x, screen[y * nScreenWidth + x]); // Render the screen buffer
                }
            }
            refresh(); // Refresh the screen to show updates

            // Wait for space to restart
            while (true) {
                int restartCh = getch(); // Wait for user input
                if (restartCh == ' ') {
                    break; // Break to restart the game
                }
            }
        }
    }

    delete[] screen; // Free allocated memory for the screen buffer
    endwin(); // End ncurses mode
}

// Main function
int main()
{
    while (true) // Outer loop for level selection
    {
        initGame(); // Initialize the game

        // Level selection
        int level = selectLevel(); // Get the selected level
        gameLoop(level); // Start the game loop with the selected level
    }
}