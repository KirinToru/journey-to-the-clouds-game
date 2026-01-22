#include "Boy.hpp"

Boy::Boy() {
  // Player appearance
  shape.setSize({30.f, 30.f});
  shape.setFillColor(sf::Color::Red);
  shape.setPosition({100.f, 0.f}); // Start position

  // Physics parameters
  moveSpeed = 300.f;
  acceleration = 2000.f;
  friction = 1000.f;

  gravity = 1200.f;
  jumpStrength = 600.f;

  // Wall mechanics
  wallSlideSpeed = 100.f;
  wallJumpForce = {400.f, 600.f};
  isWallSliding = false;
  wallDir = 0;

  velocity = {0.f, 0.f};
  isGrounded = false;
}

// Include Map for collision checks
#include "../World/Map.hpp"
#include <algorithm> // for min/max

void Boy::update(float dt, const Map &map) {
  // 1. Input Handling & Dynamic Speed (Acceleration/Friction)
  bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
  bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

  // Horizontal Movement with Acceleration
  if (left && !right) {
    velocity.x -= acceleration * dt;
  } else if (right && !left) {
    velocity.x += acceleration * dt;
  } else {
    // Friction
    if (velocity.x > 0) {
      velocity.x -= friction * dt;
      if (velocity.x < 0)
        velocity.x = 0;
    } else if (velocity.x < 0) {
      velocity.x += friction * dt;
      if (velocity.x > 0)
        velocity.x = 0;
    }
  }

  // Cap speed
  if (velocity.x > moveSpeed)
    velocity.x = moveSpeed;
  if (velocity.x < -moveSpeed)
    velocity.x = -moveSpeed;

  // 2. Wall Detection Logic
  // Check for wall one pixel ahead
  sf::FloatRect bounds = shape.getGlobalBounds();
  sf::FloatRect leftCheck = bounds;
  leftCheck.position.x -= 2.f;
  sf::FloatRect rightCheck = bounds;
  rightCheck.position.x += 2.f;

  bool touchingLeft = !map.checkCollision(leftCheck).empty();
  bool touchingRight = !map.checkCollision(rightCheck).empty();

  // Reset wall state
  isWallSliding = false;
  wallDir = 0;

  if (touchingLeft)
    wallDir = -1;
  if (touchingRight)
    wallDir = 1;

  // Wall Slide
  // Only slide if moving down, not grounded, and pressing towards wall
  if (wallDir != 0 && velocity.y > 0 && !isGrounded) {
    // Must be pressing towards the wall or just touching?
    // Usually "hugging" the wall is required or just touching.
    // Let's require pressing logic for more control, or just touching for
    // simple stickiness. Meat Boy style: sticky if pressing against it or
    // momentum carries you. For now, let's say if we are consistently pushing
    // against it or falling past it. Let's implement sticking if velocity.y > 0

    // Check input direction to stick?
    if ((wallDir == -1 && left) || (wallDir == 1 && right)) {
      isWallSliding = true;
      if (velocity.y > wallSlideSpeed) {
        velocity.y = wallSlideSpeed;
      }
    }
  }

  // 3. Jump and Wall Jump
  // Only trigger on rising edge (first frame of press)
  bool jumpJustPressed = jumpPressed && !wasJumpPressed;

  if (jumpJustPressed) {
    // Normal Jump
    if (isGrounded) {
      velocity.y = -jumpStrength;
      isGrounded = false;
    }
    // Wall Jump
    else if (isWallSliding || (wallDir != 0 && !isGrounded)) {
      velocity.y = -wallJumpForce.y;
      velocity.x = -wallDir * wallJumpForce.x;
    }
  }

  // Update previous state for next frame
  wasJumpPressed = jumpPressed;

  // 4. Variable Gravity (Dynamic Acceleration)
  float currentGravity = gravity;

  // Variable gravity relies on holding the button, so we check jumpPressed
  // directly
  if (velocity.y < 0.f && !jumpPressed) {
    // Rising but button released: heavier gravity (shorter jump)
    currentGravity *= 2.0f;
  } else if (velocity.y > 0.f) {
    // Falling: heavier gravity (fast fall), unless sliding
    if (!isWallSliding) {
      currentGravity *= 2.5f;
    } else {
      currentGravity = 0; // Handled by slide constant speed
    }
  }

  velocity.y += currentGravity * dt;

  // 5. Physics & Collision Resolution

  // --- X-AXIS ---
  shape.move({velocity.x * dt, 0.f});

  // Check collisions after X move
  std::vector<sf::FloatRect> walls =
      map.checkCollision(shape.getGlobalBounds());
  for (const auto &wall : walls) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // Calculate Intersection Overlap using SFML 3 struct members
    float overlapY = std::min(playerBounds.position.y + playerBounds.size.y,
                              wall.position.y + wall.size.y) -
                     std::max(playerBounds.position.y, wall.position.y);

    // Ignore "snagging" on floor/ceiling seams
    if (overlapY < 5.f)
      continue;

    // Resolve X collision
    if (velocity.x > 0) { // Moving Right
      shape.setPosition(
          {wall.position.x - shape.getSize().x, shape.getPosition().y});
      velocity.x = 0;            // Stop on wall
    } else if (velocity.x < 0) { // Moving Left
      shape.setPosition({wall.position.x + wall.size.x, shape.getPosition().y});
      velocity.x = 0; // Stop on wall
    }
  }

  // --- Y-AXIS ---
  // Reset grounded (will be set true if we land on something)
  isGrounded = false;

  shape.move({0.f, velocity.y * dt});

  // Check collisions after Y move
  walls = map.checkCollision(shape.getGlobalBounds());
  for (const auto &wall : walls) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // Calculate Intersection Overlap X to distinguish Wall from Floor
    float overlapX = std::min(playerBounds.position.x + playerBounds.size.x,
                              wall.position.x + wall.size.x) -
                     std::max(playerBounds.position.x, wall.position.x);

    // Ignore walls (vertical surfaces) when resolving Y collisions
    if (overlapX < 2.f)
      continue;

    // Resolve Y collision
    if (velocity.y > 0) { // Falling
      shape.setPosition(
          {shape.getPosition().x, wall.position.y - shape.getSize().y});
      velocity.y = 0.f;
      isGrounded = true;
    } else if (velocity.y < 0) { // Jumping up
      shape.setPosition({shape.getPosition().x, wall.position.y + wall.size.y});
      velocity.y = 0.f;
    }
  }
}

void Boy::render(sf::RenderWindow &window) { window.draw(shape); }

void Boy::reset(sf::Vector2f position) {
  shape.setPosition(position);
  velocity = {0.f, 0.f};
  isGrounded = false;
}