# 🗺️ Journey to the Clouds - Development Roadmap

> Step-by-step development roadmap for the Journey to the Clouds project.  
> Each milestone must be completed and tested before moving on.

---

## 📌 Status Legend

| Symbol | Meaning |
|--------|---------|
| ⬜ | Not started |
| 🔶 | In progress |
| ✅ | Completed |

---

# PHASE 1: "FOUNDATION" - Core Mechanics & Systems

> **Goal:** Establish a robust engine, perfect the "game feel" of movement, and create the technical baseline.  
> **Status:** ✅ Completed

## Milestone 0: Project Setup ✅

| # | Task | Status |
|---|------|--------|
| 0.1 | C++20 & SFML 3.0.2 Boilerplate | ✅ |
| 0.2 | CMake Build System | ✅ |
| 0.3 | GitHub Actions / Release Workflow (Windows) | ✅ |
| 0.4 | Initial state machine (Game/Menu/Pause) | ✅ |
| 0.5 | Multiplatform Support (Linux, macOS) | ✅ |

## Milestone 1: Core Movement 🔶

| # | Task | Status |
|---|------|--------|
| 1.1 | Physics-based movement (acceleration, friction, gravity) | ✅ |
| 1.2 | Variable jump height | ✅ |
| 1.3 | Wall jump & Wall slide (Dynamic slide speed) | 🔶 |
| 1.4 | Anti-snagging, corner correction, sub-pixel jitter fix | ✅ |
| 1.5 | Dash (Shift, multidirectional, single air dash) | ⬜ |
| 1.6 | Momentum Conservation (Dash -> Jump) | ⬜ |
| 1.7 | Input Buffering & Coyote Time | ⬜ |

## Milestone 2: Level Loading & World ✅

| # | Task | Status |
|---|------|--------|
| 2.1 | Dynamic TMX map parsing | ✅ |
| 2.2 | Layer and tileset system (Functional & Cosmetic) | ✅ |
| 2.3 | One-way platforms (drop-through mechanism) | ✅ |
| 2.4 | Death triggers (Spikes) & Level finish line | ✅ |

---

# PHASE 2: "OBSTACLES" - Hazards, Enemies & Interactivity

> **Goal:** Introduce dangerous elements, interactive world objects, and challenging enemies.  
> **Status:** 🔶 In Progress

## Milestone 3: Dynamic Hazards 🔶

| # | Task | Status |
|---|------|--------|
| 3.1 | Moving Blocks / Platforms | ⬜ |
| 3.2 | Saw Blades (rotating, moving along paths) | ⬜ |
| 3.3 | Boulders (rolling/falling physics) | ⬜ |
| 3.4 | Destructible Blocks (break on contact/dash) | ⬜ |
| 3.5 | Trampolines / Bounce Pads (launch player upwards) | ⬜ |
| 3.6 | Doors / Portals (transition to "behind the level" layer) | ⬜ |

## Milestone 4: Enemy System ⬜

| # | Task | Status |
|---|------|--------|
| 4.1 | Base enemy AI controller | ⬜ |
| 4.2 | Ground Patrol Enemy (walks back and forth) | ⬜ |
| 4.3 | Flying Enemy (follows player or patrols air) | ⬜ |
| 4.4 | Chaser Enemy (reacts to player proximity) | ⬜ |
| 4.5 | Boss Encounters framework | ⬜ |

## Milestone 5: Player Consequences ⬜

| # | Task | Status |
|---|------|--------|
| 5.1 | Lives System (9 lives visual indicator) | ⬜ |
| 5.2 | Full restart upon losing all lives | ⬜ |
| 5.3 | Procedural Death Effects (physics-based projectiles/blood scatter) | ⬜ |

---

# PHASE 3: "AESTHETICS" - Visuals, Shaders & Audio

> **Goal:** Enhance the game's atmosphere with post-processing, dynamic lighting, and immersive sounds.  
> **Status:** ⬜ Not Started

## Milestone 6: Shaders & Visual Effects ⬜

| # | Task | Status |
|---|------|--------|
| 6.1 | Dynamic Lighting System (e.g. Candle library) | ⬜ |
| 6.2 | Post-processing Shaders (Bloom, CRT effect, Vignette) | ⬜ |
| 6.3 | Custom Shader integration via SFML | ⬜ |
| 6.4 | Ambient Particle Systems (dust on jump, atmospheric motes) | ⬜ |

## Milestone 7: Audio Integration ⬜

| # | Task | Status |
|---|------|--------|
| 7.1 | Core Audio Manager | ⬜ |
| 7.2 | Background/Ambient Music per world (Hell, Earth, etc.) | ⬜ |
| 7.3 | Sound Effects (jump, death, block break, UI clicks) | ⬜ |

---

# PHASE 4: "CONTENT" - Levels, Story & UI

> **Goal:** Build the actual game progression, menus, and narrative wrapping.  
> **Status:** ⬜ Not Started

## Milestone 8: Menu & UI Polish ⬜

| # | Task | Status |
|---|------|--------|
| 8.1 | Level Select Screen | ⬜ |
| 8.2 | Game Over / Victory Screens | ⬜ |
| 8.3 | In-game HUD (lives, timer) | ⬜ |
| 8.4 | Settings Menu (Volume, Graphics offsets, Configurable Controls) | ⬜ |

## Milestone 9: World Progression ⬜

| # | Task | Status |
|---|------|--------|
| 9.1 | Hell Levels | ⬜ |
| 9.2 | Earth Levels (New mechanics: Wind/Stamina?) | ⬜ |
| 9.3 | Sky Levels | ⬜ |
| 9.4 | Heaven Levels (Final Ascent) | ⬜ |
| 9.5 | Lore text objects / Narrative cutscenes | ⬜ |
| 9.6 | Main Ending Sequence | ⬜ |

---

# PHASE 5: "ADVANCED" - Optimization & Extras

> **Goal:** Push the engine to maximum efficiency and provide replayability.  
> **Status:** ⬜ Not Started

## Milestone 10: Performance Optimization ⬜

| # | Task | Status |
|---|------|--------|
| 10.1 | Multithreading (Separate physics / rendering threads) | ⬜ |
| 10.2 | Assembly Modules / SIMD for collision & math | ⬜ |
| 10.3 | Texture Atlasing to reduce draw calls | ⬜ |
| 10.4 | Spatial Partitioning for heavy entity scenarios | ⬜ |

## Milestone 11: End-Game Extras ⬜

| # | Task | Status |
|---|------|--------|
| 11.1 | Speedrun Mode (Timers, split displays, ghost recording) | ⬜ |
| 11.2 | Hardcore Mode (1 life, instant restart) | ⬜ |
| 11.3 | New Game+ (Modified layouts, faster enemies) | ⬜ |
| 11.4 | Save System for progression | ⬜ |

---

> **Last Updated:** 2026-03-10  
> **Current Status:** Phase 1 ✅ - Phase 2 🔶
