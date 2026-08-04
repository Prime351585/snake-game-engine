// ---------------------------------------------------------------------------
//  input.cpp — Keyboard input handling (Windows)
//
//  Fixes the "arrow keys don't work on first press" issue by:
//    1. Using else-if so only ONE direction change is accepted per poll.
//    2. Polling at 10 ms intervals throughout the entire tick, so brief
//       key taps are never missed.
// ---------------------------------------------------------------------------

#include "input.h"

#include <windows.h>
#include <chrono>
#include <thread>

// ---------------------------------------------------------------------------
//  Single poll
// ---------------------------------------------------------------------------

void handleInputOnce() {
    SnakeDirection& dir = currentDirection();

    if ((GetAsyncKeyState(VK_UP) & 0x8000) && dir != SnakeDirection::DOWN)
        dir = SnakeDirection::UP;
    else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && dir != SnakeDirection::UP)
        dir = SnakeDirection::DOWN;
    else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && dir != SnakeDirection::RIGHT)
        dir = SnakeDirection::LEFT;
    else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && dir != SnakeDirection::LEFT)
        dir = SnakeDirection::RIGHT;
}

// ---------------------------------------------------------------------------
//  High-frequency poll over the full tick duration
// ---------------------------------------------------------------------------

void pollInputDuringTick(int tick_ms) {
    constexpr int POLL_INTERVAL_MS = 10;

    auto start = std::chrono::steady_clock::now();

    while (true) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start
        ).count();

        if (elapsed >= tick_ms) break;

        handleInputOnce();

        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
    }
}
