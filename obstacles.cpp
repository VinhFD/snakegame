#include "snake.h"
#include <random>

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