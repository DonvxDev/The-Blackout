# The Blackout Shooter Game

## Quick Setup Guide

### 1. Install Node.js
Download from https://nodejs.org/

### 2. Install Dependencies
```bash
npm install
```

### 3. Build the Game
```bash
g++ -std=c++17 -o game/game.exe main.cpp
```

### 4. Run the App
```bash
npm start
```

### 5. Create GitHub Release

```bash
# Initialize git repo
git init
git add .
git commit -m "Initial commit"
git remote add origin https://github.com/YOUR_USERNAME/the-blackout.git
git branch -M main
git push -u origin main

# Create a release tag
git tag v1.0.0
git push origin v1.0.0
```

GitHub Actions will automatically:
- Build the C++ game
- Package the Electron app
- Create a release with downloadable `.exe`

### 6. Share Your Game

Share the GitHub link so others can:
- Download the game
- See the source code
- Submit issues/PRs

---

## Folder Structure After Setup

```
the-blackout/
├── game/
│   └── game.exe          (Your compiled game)
├── node_modules/         (Electron dependencies)
├── dist/                 (Built app - auto-generated)
├── main.js              (Electron entry)
├── index.html           (Game UI)
├── preload.js           (IPC bridge)
├── package.json         (Config)
└── .github/workflows/   (CI/CD)
```

---

Done! Your Electron game is ready to deploy! 🚀
