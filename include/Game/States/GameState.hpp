#pragma once

#include <Engine/States/State.hpp>
#include <Game/Entities/Player.hpp>
#include <Game/World/Map.hpp>
#include <SFML/Graphics.hpp>

class GameState : public State {
public:
  GameState(Game *game);

  void handleInput(sf::Event &event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderWindow &window) override;

private:
  void toggleHitbox();
  void toggleFPS();
  void loadLevel(const std::string &filename);

  Player mPlayer;
  Map mMap;
  sf::View mCamera;

  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;

  // Debug features
  bool mShowHitbox;
  bool mShowFPS;

  // FPS counter
  sf::Font mFPSFont;
  bool mFPSFontLoaded;
  sf::Clock mFPSClock;
  int mFrameCount;
  int mCurrentFPS;

  // Smart Reset
  sf::RectangleShape mFadeOverlay;
  float mResetTimer;
  bool mIsResetting;

  // Death Sequence
  int mDeathPhase; // 0=none, 1=fade out, 2=hold black, 3=fade in
  float mDeathTimer;
  sf::Vector2f mDeathPosition;

  // Level System
  std::vector<std::string> mLevels;
  int mCurrentLevelIndex;
};
