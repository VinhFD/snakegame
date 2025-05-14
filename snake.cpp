#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <vector>

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
    int x;
    int y;
};

void drawBarriers(wchar_t *screen) {
    // Define barrier positions
    std::vector<std::pair<int, int>> barriers = {
        {20, 5}, {20, 6}, {20, 7}, {20, 8}, {20, 9},
        {40, 10}, {41, 10}, {42, 10}, {43, 10}, {44, 10},
        {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15},
        {80, 20}, {80, 21}, {80, 22}, {80, 23}, {80, 24}
    };

    for (const auto& barrier : barriers) {
        screen[barrier.second * nScreenWidth + barrier.first] = L'#'; // Use '#' for barriers
    }
}

int main()
{
    initscr();
    keypad(stdscr, TRUE);
    noecho(); 
    curs_set(0);

    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        screen[i] = L' ';
    }

    while (1)
    {
        std::list<sSnakeSegment> snake = {{60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15}};
        int nFoodX = 30;
        int nFoodY = 15;
        int nScore = 0;
        int nSnakeDirection = 3;
        bool bDead = false;
        timeout(100);

        while (!bDead)
        {
            auto t1 = std::chrono::system_clock::now();
            while ((std::chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? std::chrono::milliseconds(120) : std::chrono::milliseconds(200)))
            {
                int ch = getch();
                switch (ch)
                {
                    case 'q':
                        endwin();
                        return 0;
                    case KEY_LEFT:
                        if (nSnakeDirection != 1) nSnakeDirection = 3;
                        break;
                    case KEY_RIGHT:
                        if (nSnakeDirection != 3) nSnakeDirection = 1;
                        break;
                    case KEY_UP:
                        if (nSnakeDirection != 2) nSnakeDirection = 0;
                        break;
                    case KEY_DOWN:
                        if (nSnakeDirection != 0) nSnakeDirection = 2;
                        break;
                }

                switch (nSnakeDirection)
                {
                case 0: 
                    snake.push_front({snake.front().x, snake.front().y - 1});
                    break;
                case 1: 
                    snake.push_front({snake.front().x + 1, snake.front().y });
                    break;
                case 2: 
                    snake.push_front({snake.front().x, snake.front().y + 1});
                    break;
                case 3: 
                    snake.push_front({snake.front().x - 1, snake.front().y});
                    break;
                default:
                    break;
                }

                if (snake.front().x == nFoodX && snake.front().y == nFoodY)
                {
                    nScore++;
                    while (screen[nFoodY * nScreenWidth + nFoodX] != ' ')
                    {
                        nFoodX = rand() % nScreenWidth;
                        nFoodY = (rand() % (nScreenHeight - 3)) + 3;
                    }

                    for (int i = 0; i < 5; i++)
                    {
                        snake.push_back({snake.back().x, snake.back().y});
                    }
                }
            
                if (snake.front().x < 0 || snake.front().x >= nScreenWidth ||
                    snake.front().y < 3 || snake.front().y >= nScreenHeight)
                {
                    bDead = true;
                }

                // Check for collision with barriers
                std::vector<std::pair<int, int>> barriers = {
                    {20, 5}, {20, 6}, {20, 7}, {20, 8}, {20, 9},
        {40, 10}, {41, 10}, {42, 10}, {43, 10}, {44, 10},
        {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15},
        {80, 20}, {80, 21}, {80, 22}, {80, 23}, {80, 24}
    };
};