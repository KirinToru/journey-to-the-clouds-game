#pragma once

#include "Entities/Player.hpp"
#include "World/Map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Game {
public:
  Game();
  void run();

private:
  void processEvents();
  void update(sf::Time dt);
  void render();
  void loadLevel(const std::string &filename);
  void cycleWindowMode(); // F4 - cycle through window modes

  sf::RenderWindow mWindow;
  sf::View mCamera;

  Player mPlayer;
  Map mMap;

  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;

  static const sf::Time TimePerFrame;

  // Debug features
  bool mShowHitbox = false; // F1 toggle
  bool mShowFPS = false;    // F2 toggle
  int mWindowMode = 0;      // 0=windowed, 1=maximized, 2=fullscreen

  // FPS counter
  sf::Font mFPSFont;
  bool mFPSFontLoaded = false;
  sf::Clock mFPSClock;
  int mFrameCount = 0;
  int mCurrentFPS = 0;
};
