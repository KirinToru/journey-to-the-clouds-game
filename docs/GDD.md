# Game Design Document - Tech & Physics Showcase

> Living document describing the design, mechanics, and technical architecture of the game

---

## 1. Overview

| Property | Value |
|----------|-------|
| **Title** | Journey to the Clouds |
| **Genre** | 2D Platformer / Tag (Hide-and-Seek) & Physics Sandbox |
| **Platform** | Windows x64 |
| **Engine** | Custom (C++20 + SFML 3.0.2) + Custom Rigid Body Physics |
| **Art Style** | Retro Pixel Art (32×32 tiles) |

---

## 2. Core Gameplay Loop

**Objective:** Survive for 15 seconds against a self-learning AI monster in a closed physics-based arena.

**Loop Iteration:**
1. **Spawn:** Player and AI Monster spawn in the arena.
2. **Survival & Hiding Phase:** The game acts as a game of Tag (berek / ganiany). The player must evade and hide from the monster for 15 seconds. The player can break Line of Sight by hiding behind walls or physics objects.
3. **Outcome:**
   - **Win:** Player survives 15 seconds without being touched. The AI gathers data (e.g., how close it got, paths taken).
   - **Loss:** Monster touches the player. The AI gathers data on its successful strategy.
4. **Learning Phase:** The AI adjusts its internal model based on the reward (time taken to catch player or distance to player if failed).
5. **Reset:** The arena resets, and a new round begins with the smarter AI.

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

### 3.2 Advanced Player Mechanics

- **Dash:** 4-cardinal-direction burst of speed. Vertical input takes priority.
- **Air Jump:** If the player leaves the ground without jumping, they get one free jump in mid-air.
- **Momentum Conservation:** Jumping out of a dash preserves the dash speed.
- **Wall Jump / Wall Slide:** Standard 2D platformer wall mechanics.

---

## 4. Physics System (New)

The game incorporates advanced rigid body physics to allow emergent gameplay and complex interactions.

### 4.1 Rigid Body Dynamics
- **Pushable Blocks:** The player can push objects by walking into them. The speed and effort depend on the object's mass and friction.
- **Rotation & Center of Mass:** Objects can rotate. Standing on the edge of a balanced block will cause it to tip over.
- **Gravity & Free Fall:** Dropping objects from a height will cause them to fall realistically, spinning and accelerating due to gravity.
- **Restitution (Bounciness):** Objects bounce off the ground and walls depending on their material properties (e.g., rubber blocks bounce, heavy metal blocks thud and stop).
- **Angular Momentum:** Throwing or hitting objects off-center imparts spin, which affects how they bounce.

### 4.2 Interactions
- **Building & Hiding:** The player can move objects to build towers, reach higher areas, or create barricades to hide behind. The AI monster can also interact with and move these objects to search for the hidden player.

---

## 5. Self-Learning AI Monster

The antagonist is a single monster that learns and adapts to the player's strategies over multiple iterations.

### 5.1 AI Capabilities
- **Movement:** Can run, jump, and interact with physics objects (e.g., pushing barricades to reach the player).
- **Vision & Memory:** The AI has a defined Field of View. If it sees the player, it enters a Chase state. If the player breaks Line of Sight (e.g., hides behind a wall), the AI remembers the last seen location and goes there. If the player is no longer there, the AI returns to a Search state.
- **Goal:** Find and make contact with the player within 15 seconds.

### 5.2 Learning Mechanism
- **Reinforcement Learning:** The AI evaluates its performance at the end of each round.
  - **Reward:** Catching the player quickly yields a high reward.
  - **Penalty:** Failing to catch the player or taking too long yields a penalty or low reward.
- **Adaptation:** After every level reset (whether player wins or loses), the AI updates its weights (e.g., Neural Network or Q-Table) to optimize its pathing, jumping timing, and prediction of player movement.
- **Progression:** Early rounds feature a dumb, easily avoidable monster. Later rounds feature a highly aggressive, predictive, and agile monster.

---

## 6. Controls

| Key | Action |
|-----|--------|
| **A / D / ← / →** | Move left / right |
| **Space** | Jump |
| **Shift** | Dash |
| **S / ↓** | Drop through platforms / Fast Wall Slide |
| **E / Mouse** | Interact / Grab physics objects (Planned) |
| **R (hold)** | Reset level & AI |
| **` (Backtick)** | Open Developer Console (Planned) |
| **F1** | Toggle hitbox display (Will be moved to console later) |
| **F2** | Toggle developer HUD (Will be moved to console later) |
| **F4** | Toggle window mode |
| **Alt+F4** | Close game |
| **ESC** | Pause / Exit |

---

## 7. Technical Architecture

- **Custom Physics Engine:** The physics engine is written entirely from scratch in C++. It will expand on the existing AABB collision to support Rigid Body dynamics, rotation, and continuous collision detection without external libraries.
- **AI Backend:** Custom implementation of Q-Learning (Reinforcement Learning) entirely in C++. This avoids external dependencies and integrates natively with the game loop. Future extensions might explore Python or Rust bindings.
- **Game Loop:** Fixed timestep is critical for deterministic physics and consistent AI training.
