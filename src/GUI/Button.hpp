#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
  Button(const sf::Font &font, const std::string &text, sf::Vector2f position);
  void select(bool selected);
  void setPosition(sf::Vector2f position);
  bool contains(sf::Vector2f point) const;
  void render(sf::RenderWindow &window);

private:
  sf::Text mText;
  bool mIsSelected;
};