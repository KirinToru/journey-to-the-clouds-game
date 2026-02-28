<p align="center">
  <img src="assets/logo.png" alt="Journey to the Clouds" height="100px"/>
</p>
<h1 align="center">Journey to the Clouds</h1>
<h3 align="center">A 2D platformer game built with SFML</h3>
<p align="center">
<img src="https://img.shields.io/badge/C++-20-00599C?style=flat-square&logo=c%2B%2B">
<img src="https://img.shields.io/badge/SFML-v3.0.2-8CC445?logo=SFML&style=flat-square">
<img src="https://img.shields.io/badge/Platform-Windows-blue?style=flat-square">
<a href="LICENSE"><img src="https://img.shields.io/badge/license-Source%20Available-informational?style=flat-square"/></a>
</p>

Journey to the Clouds is a 2D platformer where you play as **Grim**, a demon who leaves Hell to reach Heaven. Built with C++20 and SFML 3.0.2.

### Contents

- [Features](#features)
- [Controls](#controls)
- [Tech Stack](#tech-stack)
- [Build](#build)
- [Documentation](#documentation)
- [Generating API Docs (Doxygen)](#generating-api-docs-doxygen)
- [Used Resources](#used-resources)
- [License](#license)

## Features

- 🎮 Physics-based movement with acceleration and friction
- 🦘 Wall jumping and wall sliding
- 🪜 One-way platforms (drop through with S)
- 🗺️ Dynamic TMX map loading (Tiled editor)
- ✨ Character animations (idle, walk, run, jump, fall)
- 🎯 Debug hitbox visualization (F1)
- ⏱️ FPS counter (F2)
- 🖥️ Window mode cycling (F4)

## Controls

| Key | Action |
|-----|--------|
| **A / D** | Move left / right |
| **W / Space** | Jump |
| **S** | Drop through platforms |
| **R (hold)** | Smart reset |
| **F1** | Toggle hitbox display |
| **F2** | Toggle FPS display |
| **F4** | Cycle window mode |
| **Esc** | Pause / Exit |

## Tech Stack

| Category | Tool |
|----------|------|
| Standard | C++20 |
| IDE | Visual Studio Code, Google Antigravity |
| Build System | CMake 4.2.1 |
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

## Documentation

For detailed information about the game's design, mechanics, and architecture:

- 📘 **[Game Design Document](docs/GDD.md)** - Full game design specification
- 🗺️ **[Roadmap](docs/ROADMAP.md)** - Planned features and progress tracking

## Generating API Docs (Doxygen)

This project includes a [Doxyfile](docs/Doxyfile) for generating HTML API documentation from source code comments.

### Prerequisites

1. Install [Doxygen](https://www.doxygen.nl/download.html)
2. *(Optional)* Install [Graphviz](https://graphviz.org/download/) for UML diagrams and call graphs

### Generate Docs

Run the following command from the **project root** directory:

```shell
doxygen docs/Doxyfile
```

The generated HTML documentation will be placed in `docs/doxygen/html/`. Open `docs/doxygen/html/index.html` in your browser to view it.

> **Note:** If Graphviz is not installed, Doxygen will still generate docs but skip all graph visualizations. To disable graphs explicitly, set `HAVE_DOT = NO` in the Doxyfile.

## Used Resources

### Documentation

- [SFML 3.0.2 Documentation](https://www.sfml-dev.org/documentation/3.0.2/)
- [Tiled Documentation](https://doc.mapeditor.org/en/stable/)
- [CMake Documentation](https://cmake.org/documentation/)

### Game Development Theory

- [Game Programming Patterns](https://gameprogrammingpatterns.com/contents.html)
- [LearnOpenGL](https://learnopengl.com/)

### Tools

- [SpriteFusion](https://www.spritefusion.com/) - Pixel art tileset generator

## License

This project uses a **Source Available License**. See [LICENSE](LICENSE) for full details.

- ✅ View and learn from the code
- ✅ Personal/private use
- ✅ Educational reference (with attribution)
- ❌ Commercial use (requires permission)
- ❌ Redistribution (requires permission)
- ❌ Using assets in other projects (requires permission)

This project uses the external library [SFML](https://www.sfml-dev.org/), which is licensed under the zlib/png license.
