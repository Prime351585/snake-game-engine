# Contributing to Snake Engine RGB

Thanks for your interest in contributing! This guide will help you set up the project and get started.

---

## Prerequisites

| Tool | Version | Notes |
|------|---------|-------|
| **Windows** | 10 / 11 | Required (uses `GetAsyncKeyState` for input) |
| **MSYS2** | Latest | Provides the MinGW-w64 toolchain |
| **g++** | 13+ (UCRT64) | Install via `pacman -S mingw-w64-ucrt-x86_64-gcc` |
| **OpenRGB** | 0.9+ | [Download here](https://openrgb.org/releases.html) |
| **Git** | Latest | For cloning and submodule management |

---

## Getting Started

### 1. Clone the repository

```bash
git clone --recurse-submodules https://github.com/YOUR_USERNAME/snake-engine-rgb.git
cd snake-engine-rgb
```

If you already cloned without `--recurse-submodules`:

```bash
git submodule update --init --recursive
```

### 2. Build

**Option A — PowerShell script (recommended):**

```powershell
.\scripts\build.ps1
```

**Option B — VS Code build task:**

Open the project in VS Code, press `Ctrl+Shift+B`, and select **"Build Snake Engine"**.

**Option C — Manual g++ invocation:**

```bash
C:\msys64\ucrt64\bin\g++.exe ^
  -I OpenRGB-cppSDK/include ^
  -I OpenRGB-cppSDK/external ^
  -I OpenRGB-cppSDK/external/CppUtils-Essential ^
  -I OpenRGB-cppSDK/external/CppUtils-Network ^
  -I src ^
  -g ^
  src/main.cpp src/game.cpp src/server.cpp src/input.cpp ^
  OpenRGB-cppSDK/src/Client.cpp ^
  OpenRGB-cppSDK/src/Color.cpp ^
  OpenRGB-cppSDK/src/DeviceInfo.cpp ^
  OpenRGB-cppSDK/src/Exceptions.cpp ^
  OpenRGB-cppSDK/src/MiscUtils.cpp ^
  OpenRGB-cppSDK/src/ProtocolCommon.cpp ^
  OpenRGB-cppSDK/src/ProtocolMessages.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/BinaryStream.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/ContainerUtils.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/CriticalError.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/LangUtils.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/StreamUtils.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Essential/StringUtils.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Network/HostInfo.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Network/NetAddress.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Network/Socket.cpp ^
  OpenRGB-cppSDK/external/CppUtils-Network/SystemErrorInfo.cpp ^
  -o dist/snake_game.exe ^
  -static-libgcc -static-libstdc++ -static ^
  -lws2_32
```

### 3. Run

1. Start the **OpenRGB SDK server** (see root `README.md` for detailed steps).
2. Run `dist\snake_game.exe`.

---

## Project Structure

```
src/
├── main.cpp     — Entry point (thin glue code)
├── game.h/cpp   — Snake game logic (state, movement, collision, food)
├── server.h/cpp — OpenRGB connection, device discovery, LED rendering
└── input.h/cpp  — Keyboard input handling (Windows)
```

- **game** is platform-independent (no Windows or OpenRGB headers beyond the Zone struct for matrix queries).
- **server** encapsulates all OpenRGB SDK calls in an `RGBServer` class.
- **input** handles keyboard polling with a high-frequency loop to avoid missed key presses.

---

## Code Guidelines

1. Keep game logic in `game.cpp` — it should stay portable.
2. All OpenRGB/SDK interaction goes through `RGBServer` in `server.cpp`.
3. Use `else if` (not bare `if` chains) in input handling to prevent conflicting direction changes.
4. Use `(std::max)` with parentheses to avoid Windows `max` macro collisions.
5. Use `-static` linking so the produced binary doesn't require MinGW DLLs at runtime.

---

## Pull Request Guidelines

1. Fork the repo and create a feature branch: `git checkout -b feature/my-change`
2. Make your changes.
3. Test by building and running the game with OpenRGB connected.
4. Submit a PR with a clear description of what changed and why.

---

## Ideas for Contributions

- 🎨 Custom color themes / color configuration
- 🏆 High score persistence (save to file)
- 🎵 Sound effects
- 🔄 Cross-platform input (Linux `libevdev`, macOS `CGEvent`)
- ⌨️ WASD key support
- 🐍 Multi-zone rendering (light strip + keyboard)

---

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE).
