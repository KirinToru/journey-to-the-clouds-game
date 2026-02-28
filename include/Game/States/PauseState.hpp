#pragma once

#include <Engine/GUI/Button.hpp>
#include <Engine/States/State.hpp>
#include <SFML/Graphics.hpp>
#include <vector>


class PauseState : public State {
public:
  PauseState(Game *game);

  void handleInput(sf::Event &event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderWindow &window) override;

private:
  sf::RectangleShape mBackground;
  sf::Font mFont;
  sf::Text mPauseText;

  std::vector<Button> mButtons;
  int mSelectedOptionIndex;

  void updateLayout();
};
