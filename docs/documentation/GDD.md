# 📘 Game Design Document - Journey to the Clouds

> Living document describing the design, mechanics, and technical architecture of the game.

---

## 1. Overview

| Property | Value |
|----------|-------|
| **Title** | Journey to the Clouds |
| **Genre** | 2D Platformer |
| **Platform** | Windows x64 |
| **Engine** | Custom (C++20 + SFML 3.0.2) |
| **Art Style** | Retro Pixel Art (32×32 tiles) |
| **Target Audience** | Fans of Celeste, Super Meat Boy, N++ |

---

## 2. Story & Setting

**Protagonist:** **Grim** - a demon who decides to leave his room in Hell and embark on a journey to reach Heaven.

**Progression:** The game spans four worlds, each representing a stage of Grim's ascent:

| World | Theme | Description |
|-------|-------|-------------|
| 🔥 Hell | Fire & Brimstone | Starting area, lava hazards |
| 🌍 Earth | Nature & Urban | Outdoor platforming, varied terrain |
| ☁️ Sky | Clouds & Wind | Vertical sections, wind mechanics |
| ✨ Heaven | Light & Serenity | Final area, culmination |

**Objective:** Navigate from the **bottom** of each level to the **top**, avoiding traps and hazards.

---

## 3. Core Mechanics

### 3.1 Movement

| Parameter | Value |
|-----------|-------|
| Move Speed | 400 px/s |
| Acceleration | 1500 px/s² |
| Friction | 1200 px/s² |
| Gravity | 1000 px/s² |
| Jump Strength | 500 px/s |
| Dash Speed | 750 px/s |

### 3.2 Advanced Mechanics

- **Dash (Shift/X):** 8-directional burst of speed. Only one dash allowed per air sequence; recharges on ground, wall slide, or jump.
- **Air Jump:** If the player leaves the ground without jumping (e.g. dashing upward or falling off a ledge), they get one free jump in mid-air.
- **Momentum Conservation:** Jumping out of a dash preserves the dash speed. Speed gradually decays via air friction (60% of ground friction rate).
- **Air Friction:** Horizontal momentum slowly decays while airborne, preventing infinite dash-speed coasting.
- **Coyote Time (0.1s):** Player can still jump for a brief window after running off a ledge.
- **Input Buffer (0.1s):** Jump inputs pressed just before landing are stored and executed exactly on touchdown.

### 3.3 Wall Mechanics

- **Wall Slide:** Dynamic slide speed. Reduces fall speed when pressing against a wall, but allows acceleration downwards.
- **Wall Jump:** Launches player away from wall with force (350, 500) px/s

### 3.4 Variable Jump

- Holding jump: normal gravity (floaty)
- Releasing jump early: 2× gravity (short hop)
- Peak of jump: 0.7× gravity (hang time)
- Falling: 1.8× gravity (fast fall)

### 3.4 One-Way Platforms

- Player can jump through from below
- Player lands on top
- Press **Down/S** to drop through

---

## 4. Tile System

The game uses a Tiled-based tile system with two categories of tilesets:

### 4.1 Functional Tileset (`ts_main`)

Not rendered by default (invisible barriers). Shown in debug mode (F1).

| ID | Tile | Function |
|----|------|----------|
| 0 | Start | Player spawn point (exactly one per level) |
| 1 | Finish | Level completion trigger (1px trigger edge, rotatable) |
| 2 | Block | Solid collision (invisible walls) |
| 3 | Platform | One-way platform (pass-through from below) |
| 4 | Spikes | Static hazard, instant death on contact |
| 5 | Trampoline | Bounce pad, applies massive upward velocity |
| 6 | Moving Block| Platform or block that patrols a specific path |
| 7 | Saw Blade | Dynamic hazard, instakill, follows predefined path |

### 4.2 Cosmetic Tilesets (e.g. `ts_prototype`)

Rendered visually, no collision logic. Purely decorative.

### 4.3 Flip Flags

Tiled flip flags (horizontal, vertical, diagonal) are supported for tile rotation. The Finish tile trigger edge rotates with the tile.

---

## 5. Level Design

### 5.1 Format

- **File format:** TMX (Tiled XML) with CSV-encoded layers
- **Tile size:** 32×32 pixels
- **External tilesets:** `.tsx` files in `assets/tilesets/`
- **Maps:** `.tmx` files in `assets/maps/`

### 5.2 Layer System

Layers are dynamically loaded from TMX files. Any number of layers is supported. Each layer can contain tiles from any tileset.

### 5.3 Object Layers

Support for Tiled object layers:

- **Text objects:** Rendered as in-game text with word wrapping and fade-in

### 5.4 Lives

- 9 lives per run
- Death respawns at the current level's start position
- Losing all lives: full restart (planned)

---

## 6. Controls

| Key | Action |
|-----|--------|
| **A / D / ← / →** | Move left / right |
| **W / ↑** | Dash up |
| **Space** | Jump |
| **Shift** | Dash (Directional with movement keys) |
| **S / ↓** | Drop through platforms / Fast Wall Slide |
| **R (hold)** | Reset |
| **Escape** | Pause menu |
| **F1** | Toggle hitbox display |
| **F2** | Toggle Developer HUD (FPS, velocity, state, dash) |
| **F4** | Cycle window mode (Windowed → Maximized → Fullscreen) |
| **Alt+F4** | Close game |

---

## 7. Technical Architecture

### 7.1 State Machine

The game uses a stack-based state machine:

```
Game
├── MenuState (main menu with scrolling background)
├── GameState (gameplay: map, player, camera, HUD)
│   └── PauseState (overlay with continue/restart/menu)
```

**State transitions:** Push, Pop, Change, ClearAndPush - applied at end of frame to avoid iterator invalidation.

### 7.2 Game Loop

Fixed timestep at 60 FPS (`1/60s` per frame). Accumulator pattern ensures consistent physics regardless of render speed.

### 7.3 Rendering Pipeline

1. Parallax scrolling background (camera-relative offset)
2. Map tiles (view-culled, only visible tiles rendered)
3. Entities (Enemies, moving hazards)
4. Player sprite (animated, flippable)
5. Particle Effects & Transient Objects
6. Screen-Space Shaders (Bloom, CRT, Vignette, Post-processing)
7. Debug overlays (hitboxes: green for player, purple for traps)
8. Fade overlay (for reset transitions)
9. Developer HUD (FPS with color gradient, hitbox status, screen mode, velocity, dash readiness, player state)

### 7.4 Collision System

Axis-separated collision resolution:

1. Move on X-axis → resolve X collisions
2. Move on Y-axis → resolve Y collisions
3. Check one-way platforms (only when falling, not pressing drop)

Anti-snagging: overlap thresholds prevent false collisions at tile seams.

Corner correction: when jumping into a corner, the player is nudged horizontally if possible.

### 7.5 Camera

Smooth lerp-based camera following (`speed = 5.0`). Clamped to map boundaries. Pixel-rounded to prevent sub-pixel jitter.

---

## 8. Art & Assets

| Asset | Location | Format |
|-------|----------|--------|
| Player spritesheet | `assets/player/spritesheet.png` | 32×32 per frame |
| Tilesets | `assets/tilesets/` | PNG + TSX |
| Maps | `assets/maps/` | TMX |
| Backgrounds | `assets/backgrounds/` | PNG (repeatable) |
| Fonts | `assets/fonts/` | TTF |

### 8.1 Player Animations

| State | Row | Frames | Speed |
|-------|-----|--------|-------|
| Idle | 0 | 2 | 2.0s / 0.5s |
| Walk Start | 1 | 2 | 0.15s |
| Run Loop | 2 | 4 | 0.1s |
| Stop | 3 | 2 | 0.06s - 0.1s |
| Jump | 4 | 2 | 0.1s |
| Fall | 5 | 1 | static |

---

## 9. Enemies & Hazards (Planned)

### 9.1 Hazards
| Hazard | Description |
|--------|-------------|
| **Spikes** | Static hazard. Instant death. |
| **Saw Blades** | Dynamic spinning blades on tracks. |
| **Moving Blocks** | Platforms that push or crush the player. |
| **Boulders** | Physics-enabled falling bodies. |
| **Trampolines** | Specialized blocks triggering jump momentum. |

### 9.2 Enemies
| Enemy Type | Behavior |
|------------|----------|
| **Patroller** | Walks left-right, turns at ledges. |
| **Chaser** | Runs towards player when in line of sight. |
| **Flyer** | Ignores gravity, swoops at player. |
| **Bosses** | Multi-phase encounters with unique attack patterns. |

---

## 10. Audio (Planned)

| Category | Examples |
|----------|----------|
| Music | Per-world ambient tracks |
| SFX | Jump, death, block break, spike hit |
| Ambient | Wind (Sky world), fire crackling (Hell) |

---

## 11. UI Screens

| Screen | Status | Description |
|--------|--------|-------------|
| Main Menu | ✅ Done | Start Game / Exit, scrolling background |
| Pause Menu | ✅ Done | Continue / Restart / Main Menu |
| Level Select | 🔲 Planned | Choose unlocked levels |
| Game Over | 🔲 Planned | Death count, retry options |
| Victory | 🔲 Planned | Stats, next level options |
| HUD | 🔲 Planned | Lives, timer |
| Settings | 🔲 Planned | Volume, controls, graphics |
