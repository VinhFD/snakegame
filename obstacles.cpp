#include "snake.h"
#include <random>

// Function to create a complex obstacle pattern for super hard mode
std::vector<sObstacle> createComplexObstacle() {
    std::vector<sObstacle> obstacles;
    sObstacle obs;
    
    // Generate random position for the obstacle
    std::random_device rd;
    std::mt19937 gen(rd());
    // Leave enough space for the obstacle (7x7 area)
    std::uniform_int_distribution<> disX(5, nScreenWidth - 10);
    std::uniform_int_distribution<> disY(5, nScreenHeight - 10);
    
    int startX = disX(gen);
    int startY = disY(gen);
    
    // Create a complex obstacle pattern at random position
    obs.segments.push_back({startX, startY});    
    obs.segments.push_back({startX - 1, startY}); 
    obs.segments.push_back({startX - 2, startY}); 
    obs.segments.push_back({startX - 3, startY}); 
    obs.segments.push_back({startX - 3, startY - 1}); 
    obs.segments.push_back({startX - 3, startY - 2}); 
    obs.segments.push_back({startX - 3, startY + 2}); 
    obs.segments.push_back({startX - 3, startY + 2}); 
    obs.segments.push_back({startX - 2, startY + 2}); 
    obs.segments.push_back({startX - 1, startY + 2}); 
    obs.segments.push_back({startX, startY + 2}); 
    obs.segments.push_back({startX, startY + 1}); 
    obs.segments.push_back({startX, startY - 1}); 
    obs.segments.push_back({startX, startY - 2}); 
    obs.segments.push_back({startX + 1, startY - 2}); 
    obs.segments.push_back({startX + 2, startY - 2}); 
    obs.segments.push_back({startX + 3, startY - 2}); 
    obs.segments.push_back({startX + 1, startY}); 
    obs.segments.push_back({startX + 2, startY}); 
    obs.segments.push_back({startX + 3, startY}); 
    obs.segments.push_back({startX + 3, startY + 1}); 
    obs.segments.push_back({startX + 3, startY + 2}); 
    
    obstacles.push_back(obs);
    return obstacles;
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
    std::uniform_int_distribution<> disMove(0, 1); // 50% chance for obstacle to move
    std::uniform_int_distribution<> disDir(0, 3); // 0: up, 1: right, 2: down, 3: left

    // Add complex obstacle for super hard mode (when numObstacles is 3)
    if (numObstacles == 3) { // Super hard mode
        obstacles = createComplexObstacle();
        // Make the complex obstacle stationary
        obstacles[0].isMoving = false;
    }

    // Generate random obstacles for all modes
    for (int i = 0; i < numObstacles; i++) {
        sObstacle obs;
        int startX = disX(gen);
        int startY = disY(gen);
        int type = disType(gen);
        int size = disSize(gen);

        // Only make obstacles move in hard and super hard modes
        if (numObstacles >= 2) {
            obs.isMoving = (disMove(gen) == 1); // 50% chance to move
            if (obs.isMoving) {
                obs.moveDirection = disDir(gen);
                obs.moveSpeed = 3; // Slower base speed for moving obstacles
                obs.moveCounter = 0; // Initialize move counter
            }
        }

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

// Function to update obstacle positions
void updateObstacles(std::vector<sObstacle>& obstacles) {
    for (auto& obs : obstacles) {
        if (obs.isMoving) {
            // Only move if counter reaches moveSpeed
            obs.moveCounter++;
            if (obs.moveCounter >= obs.moveSpeed) {
                obs.moveCounter = 0; // Reset counter
                
                // Check if any segment would hit the border
                bool wouldHitBorder = false;
                for (const auto& segment : obs.segments) {
                    switch (obs.moveDirection) {
                        case 0: // Up
                            if (segment.y <= 3) wouldHitBorder = true;
                            break;
                        case 1: // Right
                            if (segment.x >= nScreenWidth - 2) wouldHitBorder = true;
                            break;
                        case 2: // Down
                            if (segment.y >= nScreenHeight - 2) wouldHitBorder = true;
                            break;
                        case 3: // Left
                            if (segment.x <= 1) wouldHitBorder = true;
                            break;
                    }
                    if (wouldHitBorder) break;
                }

                // Change direction if would hit border
                if (wouldHitBorder) {
                    switch (obs.moveDirection) {
                        case 0: obs.moveDirection = 2; break; // Up -> Down
                        case 1: obs.moveDirection = 3; break; // Right -> Left
                        case 2: obs.moveDirection = 0; break; // Down -> Up
                        case 3: obs.moveDirection = 1; break; // Left -> Right
                    }
                }

                // Move each segment of the obstacle
                for (auto& segment : obs.segments) {
                    switch (obs.moveDirection) {
                        case 0: // Up
                            segment.y--;
                            break;
                        case 1: // Right
                            segment.x++;
                            break;
                        case 2: // Down
                            segment.y++;
                            break;
                        case 3: // Left
                            segment.x--;
                            break;
                    }
                }
            }
        }
    }
} 