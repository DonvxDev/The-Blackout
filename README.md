# The Blackout - Shooter Game

A professional PC shooter game built with C++ and packaged as an Electron desktop application.

## Features

- ✅ WASD Movement System with Physics
- ✅ Multiple Weapons (AK-47, Shotgun, Sniper Rifle, Rocket Launcher)
- ✅ Enemy AI (Shooters, Brutes, Scouts, Revenants)
- ✅ Dynamic Map System (Deasered Industrial, Vensiea Urban)
- ✅ Professional Physics Engine
- ✅ Optimized for 144 FPS
- ✅ Audio Management System
- ✅ Game State Management

## Installation

### Option 1: Download Release
Visit [Releases](https://github.com/yourusername/the-blackout/releases) and download the latest `.exe` file.

### Option 2: Build from Source

**Requirements:**
- Node.js 16+
- Git
- G++ compiler (MinGW)

**Build Steps:**

```bash
# Clone repository
git clone https://github.com/yourusername/the-blackout.git
cd the-blackout

# Install dependencies
npm install

# Build game
g++ -std=c++17 -o game/game.exe main.cpp

# Run
npm start
```

## How to Play

1. Launch the application
2. Click **PLAY GAME**
3. Use controls:
   - **WASD** - Move player
   - **Mouse** - Aim
   - **Space** - Fire weapon
   - **1-4** - Switch weapons

## Development

### Project Structure
```
the-blackout/
├── main.js              # Electron main process
├── preload.js           # Electron preload script
├── index.html           # UI
├── main.cpp             # Game engine (C++)
├── package.json         # Dependencies
└── .github/workflows/   # CI/CD
```

### Game Engine Architecture
- **PlayerControll** - Input handling (WASD)
- **PlayerMovement** - Physics-based movement
- **AudioManager** - Sound effects
- **GameManager** - Game state orchestration
- **Weapon System** - Multiple weapon types
- **Enemy System** - AI enemies
- **Map System** - Level management

## Creating a Release

```bash
# Tag a new version
git tag v1.0.1
git push origin v1.0.1
```

GitHub Actions will automatically build and create a release!

## System Requirements

- **OS:** Windows 10 or later
- **Processor:** Intel i5 or equivalent
- **RAM:** 4 GB
- **Storage:** 500 MB

## License

MIT License - See LICENSE file for details

## Credits

Built with:
- C++ for game engine
- Electron for desktop app
- GitHub Actions for CI/CD

---

**Coming Soon:**
- Graphics rendering (SFML/DirectX)
- Multiplayer networking
- Advanced AI
- More weapons & maps

# The-Blackout
