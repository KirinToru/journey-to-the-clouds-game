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

  sf::RenderWindow mWindow;
  sf::View mCamera;

  Player mPlayer;
  Map mMap;

  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;

  static const sf::Time TimePerFrame;
};
