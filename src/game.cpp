// ---------------------------------------------------------------------------
//  game.cpp — Snake game logic (platform-independent)
// ---------------------------------------------------------------------------

#include "game.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

// ---------------------------------------------------------------------------
//  Internal state singletons
// ---------------------------------------------------------------------------

static std::deque<Point>  s_snake;
static Point              s_food          = {0, 0};
static SnakeDirection     s_currentDir    = SnakeDirection::RIGHT;
static bool               s_gameOver      = false;
static int                s_boardWidth    = 0;
static int                s_boardHeight   = 0;
static int                s_currentSpeedMs = INITIAL_SPEED_MS;

std::deque<Point>& snakeBody()      { return s_snake; }
Point&             foodPos()        { return s_food; }
SnakeDirection&    currentDirection(){ return s_currentDir; }
bool&              gameOver()       { return s_gameOver; }
int&               boardWidth()     { return s_boardWidth; }
int&               boardHeight()    { return s_boardHeight; }
int&               currentSpeedMs() { return s_currentSpeedMs; }

// ---------------------------------------------------------------------------
//  LED / matrix helpers
// ---------------------------------------------------------------------------

unsigned int getLedIndex(const orgb::Zone& zone, int x, int y) {
    if (x >= 0 && y >= 0
        && x < static_cast<int>(zone.matrix_width)
        && y < static_cast<int>(zone.matrix_height))
    {
        const size_t index =
            static_cast<size_t>(y) * zone.matrix_width + static_cast<size_t>(x);
        if (index < zone.matrix_values.size()) {
            return zone.matrix_values[index];
        }
    }
    return 0xFFFFFFFF;
}

bool isValidLedPosition(const orgb::Zone& zone, int x, int y) {
    return getLedIndex(zone, x, y) != 0xFFFFFFFF;
}

// ---------------------------------------------------------------------------
//  Movement
// ---------------------------------------------------------------------------

Point advanceHead(const orgb::Zone& zone, Point head) {
    const int maxSteps = s_boardWidth * s_boardHeight;

    for (int step = 0; step < maxSteps; ++step) {
        switch (s_currentDir) {
            case SnakeDirection::UP:    head.y -= 1; break;
            case SnakeDirection::DOWN:  head.y += 1; break;
            case SnakeDirection::LEFT:  head.x -= 1; break;
            case SnakeDirection::RIGHT: head.x += 1; break;
        }

        // Wrap around board edges
        if (head.x < 0)             head.x = s_boardWidth  - 1;
        if (head.x >= s_boardWidth)  head.x = 0;
        if (head.y < 0)             head.y = s_boardHeight - 1;
        if (head.y >= s_boardHeight) head.y = 0;

        if (isValidLedPosition(zone, head.x, head.y)) {
            return head;
        }
    }

    return head; // fallback (should not happen on a valid keyboard)
}

// ---------------------------------------------------------------------------
//  Food
// ---------------------------------------------------------------------------

void spawnFood(const orgb::Zone& zone) {
    bool valid = false;
    while (!valid) {
        s_food.x = rand() % s_boardWidth;
        s_food.y = rand() % s_boardHeight;

        // Must land on a physical key
        if (getLedIndex(zone, s_food.x, s_food.y) == 0xFFFFFFFF)
            continue;

        // Must not overlap the snake
        valid = true;
        for (const auto& segment : s_snake) {
            if (segment == s_food) {
                valid = false;
                break;
            }
        }
    }
}

// ---------------------------------------------------------------------------
//  Game tick
// ---------------------------------------------------------------------------

void updateGame(const orgb::Zone& zone) {
    Point nextHead = advanceHead(zone, s_snake.front());

    // Self-collision check
    for (const auto& segment : s_snake) {
        if (segment == nextHead) {
            std::cout << "\n[CRASH] Self Collision at ("
                      << nextHead.x << ", " << nextHead.y << ")\n";
            s_gameOver = true;
            return;
        }
    }

    s_snake.push_front(nextHead);

    if (nextHead == s_food) {
        spawnFood(zone);
        s_currentSpeedMs = (std::max)(MIN_SPEED_MS, s_currentSpeedMs - SPEED_STEP_MS);
        std::cout << "Ate Food! Speed increased (Delay: "
                  << s_currentSpeedMs << "ms)\n";
    } else {
        s_snake.pop_back();
    }
}

// ---------------------------------------------------------------------------
//  Reset
// ---------------------------------------------------------------------------

void resetGame(const orgb::Zone& zone) {
    s_snake.clear();
    s_gameOver      = false;
    s_currentDir    = SnakeDirection::RIGHT;
    s_currentSpeedMs = INITIAL_SPEED_MS;
    s_boardWidth    = static_cast<int>(zone.matrix_width);
    s_boardHeight   = static_cast<int>(zone.matrix_height);

    // Find first valid spawn point in the middle row
    int startY = s_boardHeight / 2;
    int startX = 0;
    while (startX < s_boardWidth
           && getLedIndex(zone, startX, startY) == 0xFFFFFFFF) {
        startX++;
    }

    s_snake.push_back({startX, startY});
    spawnFood(zone);
}
