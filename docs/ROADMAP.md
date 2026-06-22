# Technical Roadmap

> Outline of the technical milestones for the Physics and AI overhaul.

---

## Phase 1: Custom Physics Engine Development

- [ ] **Math & Foundation**: Develop custom vector/matrix math required for rigid body dynamics.
- [ ] **Rigid Body Entities**: Create base classes for physics-enabled objects (Dynamic, Static, Kinematic) with properties like mass, inertia, and velocity.
- [ ] **Collision Detection**: Implement custom SAT (Separating Axis Theorem) or similar algorithms for rotated box and circle collisions.
- [ ] **Collision Resolution**: Implement custom impulse-based resolution handling restitution (bounce) and friction.
- [ ] **World Stepping**: Hook the custom physics simulation into the main game loop's fixed update.

## Phase 2: Player & Object Physics Interaction

- [ ] **Player Controller Update**: Port the existing kinematic player controller to interact seamlessly with the physics world (applying forces, impulses, or kinematic overriding).
- [ ] **Pushing & Pulling**: Implement logic for the player to push objects based on mass.
- [ ] **Weight & Rotation**: Ensure player weight affects dynamic platforms (e.g., standing on edges tips them).
- [ ] **Material Properties**: Define friction, density, and restitution (bounciness) for different tiles and objects.
- [ ] **Debug Rendering**: Integrate physics engine debug drawing to visualize rigid bodies and joints.

## Phase 3: AI Monster Baseline & Stealth Mechanics

- [ ] **Monster Entity**: Create the base entity for the monster with movement capabilities.
- [ ] **Vision System**: Implement a Field of View (FOV) and Line of Sight (LoS) checks using raycasting.
- [ ] **State Machine (Search & Chase)**: Implement AI states. The AI patrols/searches, chases when the player is seen, and investigates the last known location if the player hides.
- [ ] **15-Second Loop Logic**: Implement the level timer, win/loss conditions, and the reset sequence.

## Phase 4: Machine Learning Integration

- [x] **AI Model Selection**: Decided on custom Q-Learning implemented from scratch in C++.
- [ ] **State Representation**: Define the inputs to the AI (relative position to player, obstacles, velocities).
- [ ] **Action Space**: Define the outputs from the AI (move left, right, jump, dash).
- [ ] **Reward Function**: Implement the fitness/reward calculation at the end of each round.
- [ ] **Training Loop**: Hook the AI update step into the game loop and handle model weight adjustments between rounds.
- [ ] **Save/Load AI Brain**: Allow saving the trained model to disk so the monster's progress persists between game launches.

## Phase 5: Polish & Optimization

- [ ] **Performance Profiling**: Ensure physics simulation and AI inference run smoothly within the 16.6ms frame budget (60 FPS).
- [ ] **Visual Polish**: Add particles, effects, and animations to complement the new physics interactions.
