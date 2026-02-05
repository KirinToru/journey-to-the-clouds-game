#include "MenuState.hpp"
#include "../Game.hpp"
#include "GameState.hpp"
#include <iostream>

MenuState::MenuState(Game *game)
    : State(game), mSelectedOptionIndex(0),
      mBackgroundSprite(mBackgroundTexture), mBackgroundOffset({0.f, 0.f}) {
  if (!mFont.openFromFile("assets/fonts/font.ttf")) {
    std::cerr << "Failed to load font in MenuState!" << std::endl;
  }

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg_bricks.png")) {
    std::cerr << "Failed to load menu background!" << std::endl;
  }
  mBackgroundTexture.setRepeated(true);
  mBackgroundSprite.setTexture(mBackgroundTexture);

  // Scale to match game view (Game view = 640x360, Window = 1280x720, so 2x
  // zoom) Game background was 0.8f. To look same in menu (1x view), we need
  // 0.8f * 2 = 1.6f
  mBackgroundSprite.setScale({1.6f, 1.6f});

  // Create buttons initially
  mButtons.emplace_back(mFont, "Start Game", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Exit", sf::Vector2f{0, 0});
  mButtons[0].select(true);

  // Initial layout
  mLastWindowSize = mGame->getWindow().getSize();
  updateLayout();
}

void MenuState::updateLayout() {
  sf::Vector2u windowSize = mGame->getWindow().getSize();
  mLastWindowSize = windowSize; // Update cached size

  float centerX = windowSize.x / 2.f;
  float centerY = windowSize.y / 2.f;

  if (mButtons.size() >= 2) {
    mButtons[0].setPosition({centerX, centerY - 50.f});
    mButtons[1].setPosition({centerX, centerY + 50.f});
  }
}

void MenuState::handleInput(sf::Event &event) {
  if (event.is<sf::Event::Resized>()) {
    updateLayout();
  }

  // Mouse Input
  if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
        {mouseMove->position.x, mouseMove->position.y});

    for (int i = 0; i < mButtons.size(); ++i) {
      if (mButtons[i].contains(mousePos)) {
        // Only trigger if changed to avoid redundant calls
        if (mSelectedOptionIndex != i) {
          mButtons[mSelectedOptionIndex].select(false);
          mSelectedOptionIndex = i;
          mButtons[mSelectedOptionIndex].select(true);
        }
      }
    }
  }

  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseClick->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
          {mouseClick->position.x, mouseClick->position.y});

      // Check if clicking the currently selected button (which should be true
      // if we hovered)
      if (mButtons[mSelectedOptionIndex].contains(mousePos)) {
        if (mSelectedOptionIndex == 0) { // Start
          mGame->changeState(std::make_unique<GameState>(mGame));
        } else if (mSelectedOptionIndex == 1) { // Exit
          const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
        }
      }
    }
  }

  // Keyboard Input
  if (const auto *keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPress->code == sf::Keyboard::Key::Up ||
        keyPress->code == sf::Keyboard::Key::W) {
      mButtons[mSelectedOptionIndex].select(false);
      mSelectedOptionIndex--;
      if (mSelectedOptionIndex < 0)
        mSelectedOptionIndex = static_cast<int>(mButtons.size()) - 1;
      mButtons[mSelectedOptionIndex].select(true);
    } else if (keyPress->code == sf::Keyboard::Key::Down ||
               keyPress->code == sf::Keyboard::Key::S) {
      mButtons[mSelectedOptionIndex].select(false);
      mSelectedOptionIndex =
          (mSelectedOptionIndex + 1) % static_cast<int>(mButtons.size());
      mButtons[mSelectedOptionIndex].select(true);
    } else if (keyPress->code == sf::Keyboard::Key::Enter) {
      if (mSelectedOptionIndex == 0) { // Start
        mGame->changeState(std::make_unique<GameState>(mGame));
      } else if (mSelectedOptionIndex == 1) { // Exit
        const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
      }
    }
  }
}

void MenuState::update(sf::Time dt) {
  // Check for window resize (covers F4/Maximize where event might be missed or
  // processed elsewhere)
  sf::Vector2u currentSize = mGame->getWindow().getSize();
  if (currentSize != mLastWindowSize) {
    updateLayout();
  }

  // Diagonal movement
  float speed = 50.f;
  mBackgroundOffset.x += speed * dt.asSeconds();
  mBackgroundOffset.y -= speed * dt.asSeconds();
}

void MenuState::render(sf::RenderWindow &window) {
  // Ensure view matches window size 1:1 for UI
  sf::Vector2f size(static_cast<float>(window.getSize().x),
                    static_cast<float>(window.getSize().y));
  window.setView(sf::View(sf::FloatRect({0, 0}, size)));

  window.clear(sf::Color::Black);

  // Update Texture Rect to cover the whole window + offset
  float scale = mBackgroundSprite.getScale().x;

  int iX = static_cast<int>(mBackgroundOffset.x);
  int iY = static_cast<int>(mBackgroundOffset.y);
  int iW = static_cast<int>(size.x / scale);
  int iH = static_cast<int>(size.y / scale);

  mBackgroundSprite.setTextureRect(sf::IntRect({iX, iY}, {iW, iH}));
  window.draw(mBackgroundSprite);

  for (auto &button : mButtons) {
    button.render(window);
  }
}
