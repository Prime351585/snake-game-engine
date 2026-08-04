// ---------------------------------------------------------------------------
//  server.cpp — OpenRGB server communication
// ---------------------------------------------------------------------------

#include "server.h"

#include <iostream>

using namespace orgb;

// ---------------------------------------------------------------------------
//  Construction / Destruction
// ---------------------------------------------------------------------------

RGBServer::RGBServer()
    : m_client("Snake Engine")
{}

RGBServer::~RGBServer() = default;

// ---------------------------------------------------------------------------
//  Connection
// ---------------------------------------------------------------------------

bool RGBServer::connect(const std::string& host, uint16_t port) {
    ConnectStatus status = m_client.connect(host, port);
    if (status != ConnectStatus::Success) {
        std::cerr << "Failed to connect to OpenRGB Server: "
                  << enumString(status) << std::endl;
        return false;
    }
    std::cout << "Connected to OpenRGB server at "
              << host << ":" << port << std::endl;
    return true;
}

// ---------------------------------------------------------------------------
//  Device discovery
// ---------------------------------------------------------------------------

bool RGBServer::findKeyboard() {
    DeviceListResult result = m_client.requestDeviceList();
    if (result.status != RequestStatus::Success) {
        std::cerr << "Failed to request device list: "
                  << enumString(result.status) << std::endl;
        return false;
    }

    // Store device list so pointers remain valid
    m_deviceList = std::move(result.devices);

    m_keyboard = m_deviceList.find(DeviceType::Keyboard);
    if (!m_keyboard) {
        std::cerr << "No keyboard device found in OpenRGB!" << std::endl;
        return false;
    }

    // Look for a matrix zone
    bool foundMatrix = false;
    for (size_t i = 0; i < m_keyboard->zones.size(); ++i) {
        if (m_keyboard->zones[i].type == ZoneType::Matrix) {
            m_zoneIndex = static_cast<int>(i);
            foundMatrix = true;
            break;
        }
    }

    if (!foundMatrix) {
        std::cerr << "Keyboard has no Matrix zone — cannot render the game."
                  << std::endl;
        return false;
    }

    const Zone& zone = m_keyboard->zones[static_cast<size_t>(m_zoneIndex)];
    std::cout << "Found keyboard: " << m_keyboard->name
              << " (" << zone.matrix_width << "x" << zone.matrix_height
              << " matrix, " << m_keyboard->leds.size() << " LEDs)"
              << std::endl;

    return true;
}

// ---------------------------------------------------------------------------
//  Mode switching
// ---------------------------------------------------------------------------

bool RGBServer::setDirectMode() {
    if (!m_keyboard) return false;

    const Mode* directMode = m_keyboard->findMode("Direct");
    if (directMode) {
        RequestStatus st = m_client.changeMode(*m_keyboard, *directMode);
        if (st != RequestStatus::Success) {
            std::cerr << "Warning: could not switch to Direct mode ("
                      << enumString(st) << "). Continuing anyway.\n";
        }
    } else {
        std::cout << "Note: keyboard has no 'Direct' mode — "
                     "using current mode.\n";
    }
    return true;
}

// ---------------------------------------------------------------------------
//  Rendering
// ---------------------------------------------------------------------------

void RGBServer::renderFrame(Color snakeColor, Color foodColor) {
    if (!m_keyboard) return;

    const Zone& zone = m_keyboard->zones[static_cast<size_t>(m_zoneIndex)];

    // Start with all LEDs dark
    std::vector<Color> ledColors(m_keyboard->leds.size(), Color::Black);

    // Draw food
    unsigned int foodIdx = getLedIndex(zone, foodPos().x, foodPos().y);
    if (foodIdx != 0xFFFFFFFF) {
        ledColors[foodIdx] = foodColor;
    }

    // Draw snake
    for (const auto& seg : snakeBody()) {
        unsigned int idx = getLedIndex(zone, seg.x, seg.y);
        if (idx != 0xFFFFFFFF) {
            ledColors[idx] = snakeColor;
        }
    }

    m_client.setDeviceColors(*m_keyboard, ledColors);
}

void RGBServer::clearLEDs() {
    if (!m_keyboard) return;
    std::vector<Color> dark(m_keyboard->leds.size(), Color::Black);
    m_client.setDeviceColors(*m_keyboard, dark);
}

// ---------------------------------------------------------------------------
//  Accessors
// ---------------------------------------------------------------------------

const Zone& RGBServer::keyboardZone() const {
    return m_keyboard->zones[static_cast<size_t>(m_zoneIndex)];
}
