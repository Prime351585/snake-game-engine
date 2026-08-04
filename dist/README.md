# 🐍 Snake Engine RGB — Quick Start (Players)

Play Snake directly on your RGB keyboard!

---

## What You Need

1. **Windows 10 / 11**
2. **An RGB keyboard** supported by [OpenRGB](https://openrgb.org)
3. **OpenRGB** installed — [Download here](https://openrgb.org/releases.html)

---

## How to Play

### Step 1 — Start the OpenRGB Server

1. Open **OpenRGB**.
2. Go to the **Devices** tab — make sure your keyboard appears in the list:

![OpenRGB Devices tab showing keyboard detection](../assets/openrgb_devices_tab.png)

3. Go to the **SDK Server** tab.
4. Make sure **Server Host** is `0.0.0.0` and **Server Port** is `6742`.
5. Click **Start Server**.

![OpenRGB SDK Server tab showing host and port settings](../assets/openrgb_sdk_server.png)

> ⚠️ **Important:** If your Server Host or Port is different from what's shown above, change it to **Host: `0.0.0.0`** and **Port: `6742`**. The game won't connect otherwise.

> **Tip:** If you don't see the SDK Server tab, update to the latest version of OpenRGB.

### Step 2 — Run the Game

Double-click **`snake_game.exe`** in this folder.

A console window will appear:
```
Connected to OpenRGB server at 127.0.0.1:6742
Found keyboard: Your Keyboard Name (22x6 matrix, 110 LEDs)

========================================
  Snake Engine Running!
  Use Arrow Keys to control the snake.
  Snake = RED  |  Food = GREEN
========================================
```

### Step 3 — Play!

| Key | Action |
|-----|--------|
| ⬆️ `Up Arrow` | Move up |
| ⬇️ `Down Arrow` | Move down |
| ⬅️ `Left Arrow` | Move left |
| ➡️ `Right Arrow` | Move right |

- **🔴 Red keys** = your snake
- **🟢 Green key** = food — eat it to grow!
- The snake wraps around the edges of the keyboard.
- Game ends when you crash into yourself.
- Speed increases each time you eat food.

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| `Failed to connect to OpenRGB Server` | Make sure OpenRGB is running and the SDK Server is started |
| `No keyboard device found` | Your keyboard may not be supported by OpenRGB — check the [supported devices list](https://openrgb.org/devices_0.9.html?search=) |

> **💡 Tip:** The supported devices list may not include every device. Some keyboards work with OpenRGB even if they're not listed — download OpenRGB and check if your keyboard is detected.
| `Keyboard has no Matrix zone` | Your keyboard's LED layout isn't mapped as a matrix in OpenRGB |
| DLL errors on launch | Place `snake_game.exe` in the same folder it came in — don't move it alone |
| Keys don't respond | Click the console window to make sure it has focus |

---

## Is This Safe for My Keyboard?

**Yes, completely safe.**

- The game only sends color data to OpenRGB — it does not modify your keyboard's firmware, keymaps, or any hardware settings.
- When the game ends, all LEDs are reset to black. Restart OpenRGB or switch profiles to restore your normal lighting.
- CPU usage is negligible (< 1%).

---

## Credits

Built with [OpenRGB](https://openrgb.org) and the [OpenRGB C++ SDK](https://github.com/Youda008/OpenRGB-cppSDK).

Licensed under the MIT License.
