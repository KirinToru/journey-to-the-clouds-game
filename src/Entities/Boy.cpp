#include "Boy.hpp"

Boy::Boy() {
  // Player appearance
  shape.setSize({30.f, 30.f});
  shape.setFillColor(sf::Color::Red);
  shape.setPosition({100.f, 0.f}); // Start position

  // Physics parameters
  moveSpeed = 300.f; // Slower overall speed
  gravity = 1200.f;  // Lower base gravity for "floaty" hang time
  jumpStrength = 600.f;

  velocity = {0.f, 0.f};
  isGrounded = false;
}

// Include Map for collision checks
#include "../World/Map.hpp"
#include <algorithm> // for min/max

void Boy::update(float dt, const Map &map) {
  // 1. Reset horizontal velocity for steering
  velocity.x = 0.f;

  // 2. Input Handling
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    velocity.x = -moveSpeed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    velocity.x = moveSpeed;
  }

  // 3. Jump
  bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

  if (jumpPressed && isGrounded) {
    velocity.y = -jumpStrength;
    isGrounded = false;
  }

  // 4. Variable Gravity (Dynamic Acceleration)
  float currentGravity = gravity;

  if (velocity.y < 0.f && !jumpPressed) {
    // Rising but button released: heavier gravity (shorter jump)
    currentGravity *= 2.0f;
  } else if (velocity.y > 0.f) {
    // Falling: heavier gravity (fast fall)
    currentGravity *= 2.5f;
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
    } else if (velocity.x < 0) { // Moving Left
      shape.setPosition({wall.position.x + wall.size.x, shape.getPosition().y});
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