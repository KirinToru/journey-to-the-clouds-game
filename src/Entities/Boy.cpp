#include "Boy.hpp"

Boy::Boy() {
	// Player appearance
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Red);
	shape.setPosition({ 100.f, 0.f });  // Start position

	// Physics parameters
    moveSpeed = 400.f;
    gravity = 1500.f;
    jumpStrength = 600.f;

    velocity = { 0.f, 0.f };
    isGrounded = false;
}

void Boy::update(float dt) {
	// 1. Reset horizontal velocity
    velocity.x = 0.f;

    // 2. Steering
    // Left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity.x = -moveSpeed;
    }
    // Right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity.x = moveSpeed;
    }

    // 3. Jump
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) && isGrounded) {
        velocity.y = -jumpStrength;
        isGrounded = false;
    }

    // 4. Gravitation
    velocity.y += gravity * dt;

    // 5. Movement
    shape.move(velocity * dt);

    // 6. Floor.
    if (shape.getPosition().y + shape.getSize().y > 500.f) {
        shape.setPosition({ shape.getPosition().x, 500.f - shape.getSize().y });
        velocity.y = 0.f;
        isGrounded = true;
    }
    else {
        isGrounded = false;
    }
}

void Boy::render(sf::RenderWindow& window) {
    window.draw(shape);
}