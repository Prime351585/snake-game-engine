// ---------------------------------------------------------------------------
//  main.cpp — Snake Engine entry point
//
//  A keyboard-based Snake game rendered on OpenRGB-compatible RGB keyboards.
//  This file is intentionally thin — it wires together:
//    • server.h  — OpenRGB connection & LED rendering
//    • game.h    — Snake state, movement, collision
//    • input.h   — Keyboard input with high-frequency polling
// ---------------------------------------------------------------------------

#include "server.h"
#include "game.h"
#include "input.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // -----------------------------------------------------------------------
    //  1. Connect to OpenRGB
    // -----------------------------------------------------------------------
    RGBServer server;

    if (!server.connect()) {
        std::cerr << "\nMake sure the OpenRGB SDK server is running on "
                     "127.0.0.1:6742.\n"
                     "Open OpenRGB -> SDK Server tab -> Start Server.\n";
        return 1;
    }

    if (!server.findKeyboard()) {
        std::cerr << "\nNo compatible keyboard found.\n"
                     "Your keyboard must be detected by OpenRGB and "
                     "have a Matrix zone.\n";
        return 1;
    }

    server.setDirectMode();

    // -----------------------------------------------------------------------
    //  2. Initialise game
    // -----------------------------------------------------------------------
    const orgb::Zone& zone = server.keyboardZone();
    resetGame(zone);

    std::cout << "\n"
              << "========================================\n"
              << "  Snake Engine Running!\n"
              << "  Use Arrow Keys to control the snake.\n"
              << "  Snake = RED  |  Food = GREEN\n"
              << "========================================\n\n";

    // -----------------------------------------------------------------------
    //  3. Main game loop
    // -----------------------------------------------------------------------
    while (!gameOver()) {
        // Poll input at high frequency for the duration of the tick.
        // This replaces the old sleep_for + single poll, fixing the
        // "arrow keys don't register on first press" issue.
        pollInputDuringTick(currentSpeedMs());

        updateGame(zone);

        if (gameOver()) break;

        server.renderFrame(
            orgb::Color(255, 0, 0),   // snake: red
            orgb::Color(0, 255, 0)    // food:  green
        );
    }

    // -----------------------------------------------------------------------
    //  4. Game over
    // -----------------------------------------------------------------------
    int score = static_cast<int>(snakeBody().size()) - 1;
    std::cout << "\nGame Over! Final Score: " << score << "\n";

    server.clearLEDs();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
