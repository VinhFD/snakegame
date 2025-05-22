#include "snake.h"

int main() {
    srand(time(nullptr)); // Seed the random number generator
    initGame(); // Initialize the game

    while (true) {
        int choice = showMainMenu(); // Show the main menu and get user choice
        if (choice == 0) { // If "Game" is selected
            int level = selectLevel(); // Show level selection menu
            if (level > 0) { // If a valid level is selected
                int highscore = getHighScore(level); // Get the high score for the selected level
                int score = gameLoop(level, highscore); // Start the game loop
                if (score > highscore) { // If the current score is higher than the high score
                    saveHighScore(level, score); // Save the new high score
                }
            }
            // If level is 0, it means user pressed 'r' to return to menu
        } else if (choice == 1) { // If "Record" is selected
            showRecords(); // Show the records menu
        }
    }

    return 0;
} 