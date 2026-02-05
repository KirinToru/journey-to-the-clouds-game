#pragma once

#include <SFML/Graphics.hpp>

class Game;

class State {
protected:
  Game *mGame;

public:
  State(Game *game) : mGame(game) {}
  virtual ~State() = default;

  virtual void handleInput(sf::Event &event) = 0;
  virtual void update(sf::Time dt) = 0;
  virtual void render(sf::RenderWindow &window) = 0;
};