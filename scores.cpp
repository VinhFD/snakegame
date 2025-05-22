#include "snake.h"
#include <fstream>
#include <string>

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