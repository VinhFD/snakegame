#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <vector>
#include <random>
#include <fstream>

// Define the dimensions of the game screen
int nScreenWidth = 100; // Width of the screen
int nScreenHeight = 45; // Height of the screen

// Structure to represent a segment of the snake
struct sSnakeSegment
{
    int x; // X coordinate
    int y; // Y coordinate
};

// Structure to represent an obstacle
struct sObstacle
{
    std::vector<sSnakeSegment> segments; // Vector of segments that make up the obstacle
};

// Function to initialize ncurses
void initGame() {
    initscr(); // Start ncurses mode
    start_color(); // Start color support
    keypad(stdscr, TRUE); // Enable special keys (like arrow keys)
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor

    // Initialize color pairs
    init_pair(1, COLOR_BLUE, COLOR_BLUE); // Border color
    init_pair(2, COLOR_GREEN, COLOR_GREEN); // Snake color
    init_pair(3, COLOR_RED, COLOR_RED); // Food color
    init_pair(4, COLOR_WHITE, COLOR_BLACK); // Text color
    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA); // Obstacle color
}

// Function to generate random obstacles
std::vector<sObstacle> generateObstacles(int numObstacles) {
    std::vector<sObstacle> obstacles;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(5, nScreenWidth - 6);
    std::uniform_int_distribution<> disY(5, nScreenHeight - 6);
    std::uniform_int_distribution<> disType(0, 9); // Added L and U shapes
    std::uniform_int_distribution<> disSize(3, 8); // Random size for obstacles

    for (int i = 0; i < numObstacles; i++) {
        sObstacle obs;
        int startX = disX(gen);
        int startY = disY(gen);
        int type = disType(gen);
        int size = disSize(gen);

        switch (type) {
            case 0: // Horizontal line
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX + j, startY});
                }
                break;
            case 1: // Vertical line
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX, startY + j});
                }
                break;
            case 2: // Square
                for (int j = 0; j < size/2; j++) {
                    for (int k = 0; k < size/2; k++) {
                        obs.segments.push_back({startX + j, startY + k});
                    }
                }
                break;
            case 3: // Cross
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX + j, startY + size/2});
                    obs.segments.push_back({startX + size/2, startY + j});
                }
                break;
            case 4: // Diamond
                for (int j = 0; j < size/2; j++) {
                    obs.segments.push_back({startX + j, startY + size/2 - j});
                    obs.segments.push_back({startX + j, startY + size/2 + j});
                    obs.segments.push_back({startX + size - j, startY + size/2 - j});
                    obs.segments.push_back({startX + size - j, startY + size/2 + j});
                }
                break;
            case 5: // Zigzag
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX + j, startY + (j % 2)});
                }
                break;
            case 6: // Stairs
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX + j, startY + j});
                    obs.segments.push_back({startX + j + 1, startY + j});
                }
                break;
            case 7: // Random pattern
                for (int j = 0; j < size; j++) {
                    if (j % 3 == 0) {
                        obs.segments.push_back({startX + j, startY});
                        obs.segments.push_back({startX + j, startY + 1});
                    } else if (j % 3 == 1) {
                        obs.segments.push_back({startX + j, startY + 1});
                        obs.segments.push_back({startX + j, startY + 2});
                    } else {
                        obs.segments.push_back({startX + j, startY});
                        obs.segments.push_back({startX + j, startY + 2});
                    }
                }
                break;
            case 8: // L shape (variable size)
                // Vertical part
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX, startY + j});
                }
                // Horizontal part
                for (int j = 1; j < size/2; j++) {
                    obs.segments.push_back({startX + j, startY + size - 1});
                }
                break;
            case 9: // U shape (variable size)
                // Left vertical
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX, startY + j});
                }
                // Right vertical
                for (int j = 0; j < size; j++) {
                    obs.segments.push_back({startX + size/2, startY + j});
                }
                // Bottom horizontal
                for (int j = 1; j < size/2; j++) {
                    obs.segments.push_back({startX + j, startY + size - 1});
                }
                break;
        }
        obstacles.push_back(obs);
    }
    return obstacles;
}

// Function to get the highest score for a level from file
int getHighScore(int level) {
    std::string filename = "highscore" + std::to_string(level) + ".txt";
    std::ifstream infile(filename);
    int highscore = 0;
    if (infile.is_open()) {
        infile >> highscore;
        infile.close();
    }
    return highscore;
}

// Function to save the highest score for a level to file
void saveHighScore(int level, int score) {
    std::string filename = "highscore" + std::to_string(level) + ".txt";
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << score;
        outfile.close();
    }
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

            // Check for obstacle collision
            for (const auto& obs : obstacles) {
                for (const auto& seg : obs.segments) {
                    if (snake.front().x == seg.x && snake.front().y == seg.y) {
                        bDead = true; // Snake hits obstacle
                        break;
                    }
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

            // Draw obstacles
            for (const auto& obs : obstacles) {
                for (const auto& seg : obs.segments) {
                    screen[seg.y * nScreenWidth + seg.x] = L'#'; // Draw obstacle segments
                }
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
                    // Use color pairs for drawing
                    if (y == 0 || y == nScreenHeight - 1 || x == 0 || x == nScreenWidth - 1) {
                        attron(COLOR_PAIR(1)); // Border color
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
    return nScore;
}

// Function to display all level records
void showRecords() {
    clear();
    mvprintw(3, 5, "=== HIGH SCORES ===");
    mvprintw(5, 5, "Easy Level: %d", getHighScore(1));
    mvprintw(6, 5, "Hard Level: %d", getHighScore(2));
    mvprintw(7, 5, "Super Hard Level: %d", getHighScore(3));
    mvprintw(9, 5, "Press any key to return to menu...");
    refresh();
    getch();
}

// Function to display the main menu
int showMainMenu() {
    int choice = 1; // 1 for Game, 2 for Record
    while (true) {
        clear();
        mvprintw(5, 5, "=== SNAKE GAME ===");
        mvprintw(7, 5, "%s Game", choice == 1 ? ">" : " ");
        mvprintw(8, 5, "%s Record", choice == 2 ? ">" : " ");
        mvprintw(10, 5, "Use UP/DOWN arrows to select");
        mvprintw(11, 5, "Press ENTER to confirm");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                choice = (choice == 1) ? 2 : 1;
                break;
            case KEY_DOWN:
                choice = (choice == 1) ? 2 : 1;
                break;
            case '\n': // Enter key
                return choice;
        }
    }
}

// Main function
int main()
{
    while (true) // Outer loop for main menu
    {
        initGame(); // Initialize the game

        int menuChoice = showMainMenu();
        if (menuChoice == 1) { // Game option
            // Level selection
            int level = selectLevel(); // Get the selected level
            int highscore = getHighScore(level);
            int score = gameLoop(level, highscore); // Start the game loop with the selected level
            if (score > highscore) {
                saveHighScore(level, score);
            }
        } else { // Record option
            showRecords();
        }
    }
}