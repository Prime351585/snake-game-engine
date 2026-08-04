#ifndef SNAKE_SERVER_H
#define SNAKE_SERVER_H

#include <vector>
#include <string>
#include "OpenRGB/Client.hpp"
#include "game.h"

// ---------------------------------------------------------------------------
//  RGBServer — wraps all OpenRGB SDK interaction
// ---------------------------------------------------------------------------

class RGBServer {
public:
    RGBServer();
    ~RGBServer();

    // Non-copyable
    RGBServer(const RGBServer&) = delete;
    RGBServer& operator=(const RGBServer&) = delete;

    /// Connect to the OpenRGB SDK server.
    /// @param host  IP address or hostname (default "127.0.0.1").
    /// @param port  TCP port (default 6742).
    /// @return true on success.
    bool connect(const std::string& host = "127.0.0.1", uint16_t port = orgb::defaultPort);

    /// Discover the first keyboard device with a matrix zone.
    /// Must be called after connect().
    /// @return true if a suitable keyboard was found.
    bool findKeyboard();

    /// Switch the keyboard to "Direct" mode so we can set per-LED colors.
    /// @return true on success (or if no Direct mode exists — we proceed anyway).
    bool setDirectMode();

    /// Send a full frame of LED colors to the keyboard.
    /// @param snakeColor  Color for snake body segments.
    /// @param foodColor   Color for the food LED.
    void renderFrame(orgb::Color snakeColor, orgb::Color foodColor);

    /// Turn off all LEDs (set to black).
    void clearLEDs();

    /// Access the keyboard zone (needed by game logic for matrix queries).
    const orgb::Zone& keyboardZone() const;

private:
    orgb::Client          m_client;
    const orgb::Device*   m_keyboard   = nullptr;
    orgb::DeviceList      m_deviceList;   // owns the device memory
    int                   m_zoneIndex  = 0;
};

#endif // SNAKE_SERVER_H
