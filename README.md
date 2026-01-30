<p align="center">
  <img src="assets/logo.png" alt="Journey to the Clouds" height="200px"/>
</p>
<h1 align="center">Journey to the Clouds</h1>
<h3 align="center">A 2D platformer game built with SFML</h3>
<p align="center">
<img src="https://img.shields.io/badge/C++-20-00599C?style=flat-square&logo=c%2B%2B">
<img src="https://img.shields.io/badge/SFML-v3.0.2-8CC445?logo=SFML&style=flat-square">
<img src="https://img.shields.io/badge/Platform-Windows-blue?style=flat-square">
<a href="LICENSE"><img src="https://img.shields.io/badge/license-Source%20Available-informational?style=flat-square"/></a>
</p>

Journey to the Clouds is a 2D platformer game. Built with C++20 and SFML 3.0.2.

### Contents

- [About](#about)
- [Features](#features)
- [Controls](#controls)
- [Tech Stack](#tech-stack)
- [Build](#build)
- [Used Resources](#used-resources)
- [Roadmap](#roadmap)
- [Documentation](#documentation)
- [License](#license)

## About

**Genre:** 2D Platformer  
**Visual Style:** Retro Pixel Art  

**Story:**  
You play as **Grim**, a demon who decides to leave his room in Hell and embark on a journey to reach Heaven. Ascend through treacherous levels, avoid deadly traps, and climb from the depths of the underworld to the clouds above.

**Gameplay:**
- Navigate from the **bottom** of each level to the **top**
- You have **9 lives** — lose one and respawn at the start of the current level
- Lose all 9 lives and restart from the very beginning
- **Level Select** mode available to practice any unlocked map 

## Features

- 🎮 Smooth physics-based movement with acceleration and friction
- 🦘 Wall jumping and wall sliding mechanics
- 🪜 One-way platforms (pass through from below, drop through with S)
- 🎨 Tileset-based rendering
- 🗺️ TMX map loading (Tiled editor support)
- ✨ Character animations (idle, walk, run, stop)

## Controls

| Key | Action |
|-----|--------|
| **A / D** | Move left / right |
| **W / Space** | Jump |
| **S** | Drop through platforms |
| **F1** | Toggle hitbox display |
| **F2** | Toggle fps display |
| **F4** | Toggle fullscreen |
| **Esc** | Exit game |

## Tech Stack

| Category | Tool |
|----------|------|
| Standart | C++20 |
| IDE | Visual Studio Code, Google Antigravity |
| Build System | CMake 4.2.1|
| Compiler | MSVC (Visual Studio 17/18) |
| Library | SFML 3.0.2 |
| Renderer | OpenGL (via SFML) |
| Platform | Windows x64 |
| Map Editor | Tiled |
| Art Tools | SpriteFusion, Photoshop |
| Map Format | TMX (Tiled XML) |
| Game Loop | Fixed Timestep |
| Version Control | Git, GitHub |

## Build

### Prerequisites

- **C++20** compatible compiler
- **CMake** 3.16+
- **SFML 3.0.2** installed (set `SFML_DIR` if not in standard location)

### Windows (Visual Studio)

```shell
mkdir build && cd build
cmake .. -G "Visual Studio 18 2026"
cmake --build . --config Release
```

### CMake (General)

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

The executable will be generated in the `build` folder.

## Used Resources

### Documentations
- [SFML 3.0.2 Documentation](https://www.sfml-dev.org/documentation/3.0.2/) - SFML official docs
- [Tiled Documentation](https://doc.mapeditor.org/en/stable/) - Tiled map editor docs
- [CMake Documentation](https://cmake.org/documentation/) - CMake reference

### Game Development Theory
- [Game Programming Patterns](https://gameprogrammingpatterns.com/contents.html) - Game programming patterns
- [LearnOpenGL](https://learnopengl.com/) - OpenGL theory

### Tools
- [SpriteFusion](https://www.spritefusion.com/) - Pixel art tileset generator

## Roadmap

### 🎨 User Interface
- [ ] Main menu
- [ ] Pause menu
- [ ] Level select screen
- [ ] Game over / Victory screens
- [ ] HUD (lives counter, timer)

### ❤️ Lives System
- [ ] 9 lives per run
- [ ] Respawn at level start on death
- [ ] Full restart on losing all lives
- [ ] Visual lives indicator

### 💀 Death Effects
- [ ] Procedural projectiles on death (physics-based, bounce off walls)
- [ ] Chaotic particle scatter effect
- [ ] Generated via code, not sprites

### 🧱 Destructible Blocks
- [ ] Blocks that break on contact
- [ ] Chaotic debris with physics
- [ ] Particle scatter effect

### 🔊 Audio
- [ ] Background music
- [ ] Sound effects (jump, death, block break)
- [ ] Ambient sounds

### ⚠️ Enemies & Hazards
- [ ] Spikes
- [ ] Saw blades
- [ ] Boulders (rolling/falling)
- [ ] Moving hazards

### 🔧 Functional Blocks
- [ ] Trampolines (bounce pads)
- [ ] Doors/Portals (transition to "behind the level" layer)
- [ ] Layer switching mechanic

### 💡 Lighting System
- [ ] Dynamic lighting using [Candle](https://github.com/MiguelMJ/Candle)
- [ ] Light reacts to player and environment
- [ ] Shadow casting

### 📊 Statistics
- [ ] Level completion timer
- [ ] Total run timer
- [ ] Death counter

### 🗺️ Levels
- [ ] More levels (Hell → Earth → Sky → Heaven)
- [ ] Increasing difficulty progression
- [ ] Boss encounters

### 📖 Story & Lore
- [ ] Expand Grim's backstory
- [ ] In-game lore elements
- [ ] Ending sequence

### ⚡ Optimization
- [ ] Texture atlasing (reduce draw calls)
- [ ] Object pooling for projectiles/particles
- [ ] Spatial partitioning for collision detection
- [ ] Frustum culling improvements
- [ ] Asset preloading / loading screens

### 🎮 Quality of Life
- [ ] Settings menu (volume, controls, graphics)
- [ ] Keybinding customization
- [ ] Gamepad/Controller support
- [ ] Save system (progress, unlocks)
- [ ] Achievements

### 🌐 Extras
- [ ] Speedrun mode (ghost replays, splits)
- [ ] Hardcore mode (1 life only)
- [ ] New Game+ (harder variants)
- [ ] Leaderboards (local/online)

## License

This project uses a **Source Available License**. See [LICENSE](LICENSE) for full details.

**Summary:**
- ✅ View and learn from the code
- ✅ Personal/private use
- ✅ Educational reference (with attribution)
- ❌ Commercial use (requires permission)
- ❌ Redistribution (requires permission)
- ❌ Using assets in other projects (requires permission)

This project uses the external library [SFML](https://www.sfml-dev.org/), which is licensed under the zlib/png license.
