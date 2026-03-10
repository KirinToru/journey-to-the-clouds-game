#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
  Player();

  void update(float dt, const class Map &map);

  void render(sf::RenderWindow &window, bool showHitbox = false);

  void reset(sf::Vector2f position);

  sf::Vector2f getPosition() const { return shape.getPosition(); }
  sf::Vector2f getVelocity() const { return velocity; }
  sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

  // State Getters for HUD
  bool getIsGrounded() const { return isGrounded; }
  bool getIsDashing() const { return isDashing; }
  bool getIsWallSliding() const { return isWallSliding; }
  bool getHasAirDash() const { return hasAirDash; }
  float getDashCooldownTimer() const { return dashCooldownTimer; }

private:
  sf::RectangleShape shape;

  sf::Vector2f velocity;
  bool isGrounded;

  float moveSpeed;
  float acceleration;
  float friction;
  float gravity;
  float jumpStrength;

  float wallSlideSpeed;
  float fastWallSlideSpeed;
  sf::Vector2f wallJumpForce;
  bool isWallSliding;
  int wallDir;

  // New Mechanics
  float dashSpeed;
  float dashDuration;
  float dashTimer;
  float dashCooldown;
  float dashCooldownTimer;
  bool isDashing;
  sf::Vector2f dashDirection;
  bool hasAirDash;

  bool hasAirJump;
  bool isJumping; // true when upward velocity comes from a jump, not a dash

  float jumpBufferTime;
  float jumpBufferTimer;
  bool bufferedJump;

  float coyoteTime;
  float coyoteTimer;

  float currentMaxSpeed;
  float speedDecay;

  sf::Texture texture;
  sf::Sprite sprite;
  bool facingRight;

  // Animation
  enum class AnimState { Idle, WalkStart, RunLoop, Stopping, Jumping, Falling };
  AnimState animState;
  int currentFrame;
  float animationTimer;
  float animationSpeed; // seconds per frame
  bool wasMoving;       // Track if player was moving last frame

  bool wasJumpPressed;
};