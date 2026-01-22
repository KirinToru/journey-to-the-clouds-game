#pragma once

#include "Entities/Boy.hpp"
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

private:
  sf::RenderWindow mWindow;
  sf::View mCamera;

  Boy mPlayer;
  Map mMap;

  static const sf::Time TimePerFrame;
};
