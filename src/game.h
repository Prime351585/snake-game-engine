#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <deque>
#include <vector>
#include "OpenRGB/DeviceInfo.hpp"

// ---------------------------------------------------------------------------
//  Data types
// ---------------------------------------------------------------------------

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

enum class SnakeDirection { UP, DOWN, LEFT, RIGHT };

// ---------------------------------------------------------------------------
//  Game state  (module-internal, exposed through accessors)
// ---------------------------------------------------------------------------

/// Current snake body (front = head).
std::deque<Point>& snakeBody();

/// Current food position.
Point& foodPos();

/// Current movement direction.
SnakeDirection& currentDirection();

/// Is the game over?
bool& gameOver();

/// Board dimensions derived from the keyboard zone matrix.
int& boardWidth();
int& boardHeight();

/// Current tick delay in milliseconds (decreases as score rises).
int& currentSpeedMs();

// ---------------------------------------------------------------------------
//  Constants
// ---------------------------------------------------------------------------

constexpr int INITIAL_SPEED_MS = 220;
constexpr int MIN_SPEED_MS     = 60;
constexpr int SPEED_STEP_MS    = 10;

// ---------------------------------------------------------------------------
//  Pure game-logic functions
// ---------------------------------------------------------------------------

/// Return the linear LED index for a matrix (x, y) coordinate.
/// Returns 0xFFFFFFFF when the position has no physical key.
unsigned int getLedIndex(const orgb::Zone& zone, int x, int y);

/// Check whether (x, y) maps to a real LED.
bool isValidLedPosition(const orgb::Zone& zone, int x, int y);

/// Move the head one step in `currentDirection()`, skipping matrix gaps,
/// and wrapping around the board edges.
Point advanceHead(const orgb::Zone& zone, Point head);

/// Place food on a random valid LED that is not occupied by the snake.
void spawnFood(const orgb::Zone& zone);

/// Advance the game by one tick: move head, check collisions, eat food.
void updateGame(const orgb::Zone& zone);

/// Reset all game state for a new round.
void resetGame(const orgb::Zone& zone);

#endif // SNAKE_GAME_H
