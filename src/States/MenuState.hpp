#pragma once

#include "../GUI/Button.hpp"
#include "State.hpp"
#include <vector>

class MenuState : public State {
public:
  MenuState(Game *game);

  void handleInput(sf::Event &event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderWindow &window) override;

private:
  sf::Font mFont;
  std::vector<Button> mButtons;
  int mSelectedOptionIndex; // 0 = Start, 1 = Exit

  // Background
  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;
  sf::Vector2f mBackgroundOffset;
  sf::Vector2u mLastWindowSize;

  void updateLayout();
};
