#include "snake.h"

// Function to display all level records
void showRecords() {
    clear();
    attron(COLOR_PAIR(6)); // Cyan text
    mvprintw(3, 5, "=== HIGH SCORES ===");
    mvprintw(5, 5, "Easy Level: %d", getHighScore(1));
    mvprintw(6, 5, "Hard Level: %d", getHighScore(2));
    mvprintw(7, 5, "Super Hard Level: %d", getHighScore(3));
    mvprintw(9, 5, "Press any key to return to menu...");
    attroff(COLOR_PAIR(6));
    refresh();
    
    // Wait for any key press
    timeout(-1); // Wait indefinitely for input
    getch();
    timeout(0); // Reset timeout to non-blocking
}

// Function to display the main menu
int showMainMenu() {
    int choice = 0; // 0 for Game, 1 for Record
    while (true) {
        clear();
        attron(COLOR_PAIR(6)); // Cyan text
        mvprintw(5, 5, "=== SNAKE GAME ===");
        mvprintw(7, 5, "%s Game", choice == 0 ? ">" : " ");
        mvprintw(8, 5, "%s Record", choice == 1 ? ">" : " ");
        mvprintw(10, 5, "Use UP/DOWN arrows to select");
        mvprintw(11, 5, "Press ENTER to confirm");
        attroff(COLOR_PAIR(6));
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                choice = (choice == 0) ? 1 : 0;
                break;
            case KEY_DOWN:
                choice = (choice == 0) ? 1 : 0;
                break;
            case '\n': // Enter key
                return choice;
        }
    }
}

// Function to display the level selection menu
int selectLevel() {
    int level; // Variable to store the selected level
    while (true) {
        clear(); // Clear the screen
        attron(COLOR_PAIR(6)); // Cyan text
        mvprintw(5, 5, "Select Level:"); // Display level selection prompt
        mvprintw(6, 5, "1. Easy"); // Option for Easy level
        mvprintw(7, 5, "2. Hard"); // Option for Hard level
        mvprintw(8, 5, "3. Super Hard"); // Option for Super Hard level
        mvprintw(9, 5, "Enter your choice (1-3): "); // Prompt for user input
        mvprintw(10, 5, "Press 'r' to return to menu"); // Return to menu option
        attroff(COLOR_PAIR(6));
        refresh(); // Refresh the screen to show updates

        int ch = getch(); // Get user input
        if (ch == 'r' || ch == 'R') { // Check if 'r' is pressed
            return 0; // Return 0 to indicate return to menu
        }
        if (ch >= '1' && ch <= '3') { // Check if input is valid
            level = ch - '0'; // Convert char to int (1, 2, or 3)
            break; // Exit the loop
        }
    }
    return level; // Return the selected level
} 