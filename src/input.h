#ifndef SNAKE_INPUT_H
#define SNAKE_INPUT_H

#include "game.h"

// ---------------------------------------------------------------------------
//  Input handling
// ---------------------------------------------------------------------------

/// Poll keyboard input once and update the current direction.
/// Uses else-if to ensure only one direction change per call.
void handleInputOnce();

/// Poll input repeatedly for the duration of one game tick.
/// This ensures brief key presses are never missed — fixes the
/// "arrow keys don't work on first press" issue.
///
/// @param tick_ms  Duration of the current game tick in milliseconds.
void pollInputDuringTick(int tick_ms);

#endif // SNAKE_INPUT_H
