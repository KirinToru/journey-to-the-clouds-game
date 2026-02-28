#include <Game/Game.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/MenuState.hpp>
#include <Game/States/PauseState.hpp>
#include <iostream>

PauseState::PauseState(Game *game)
    : State(game), mPauseText(mFont), mSelectedOptionIndex(0) {
  if (!mFont.openFromFile("assets/fonts/font.ttf"))
    std::cerr << "Failed to load font in PauseState!" << std::endl;

  sf::Vector2u windowSize = mGame->getWindow().getSize();
  mBackground.setSize(static_cast<sf::Vector2f>(windowSize));
  mBackground.setFillColor(sf::Color(0, 0, 0, 150));

  mPauseText.setFont(mFont);
  mPauseText.setString("PAUSED");
  mPauseText.setCharacterSize(50);
  sf::FloatRect textBounds = mPauseText.getLocalBounds();
  mPauseText.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
  mPauseText.setPosition({static_cast<float>(windowSize.x) / 2.f,
                          static_cast<float>(windowSize.y) / 4.f});

  mButtons.emplace_back(mFont, "Continue", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Restart", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Main Menu", sf::Vector2f{0, 0});

  updateLayout();
  mButtons[0].select(true);
}

void PauseState::updateLayout() {
  sf::Vector2u windowSize = mGame->getWindow().getSize();
  mBackground.setSize(static_cast<sf::Vector2f>(windowSize));

  float centerX = windowSize.x / 2.f;
  float centerY = windowSize.y / 2.f;

  if (mButtons.size() >= 3) {
    mButtons[0].setPosition({centerX, centerY - 40.f});
    mButtons[1].setPosition({centerX, centerY + 20.f});
    mButtons[2].setPosition({centerX, centerY + 80.f});
  }

  mPauseText.setPosition({centerX, centerY - 120.f});
}

void PauseState::handleInput(sf::Event &event) {
  if (event.is<sf::Event::Resized>())
    updateLayout();

  if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
        {mouseMove->position.x, mouseMove->position.y},
        mGame->getWindow().getDefaultView());

    for (int i = 0; i < mButtons.size(); ++i) {
      if (mButtons[i].contains(mousePos)) {
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
          {mouseClick->position.x, mouseClick->position.y},
          mGame->getWindow().getDefaultView());

      if (mButtons[mSelectedOptionIndex].contains(mousePos)) {
        if (mSelectedOptionIndex == 0) {
          mGame->popState();
        } else if (mSelectedOptionIndex == 1) {
          mGame->popState();
          mGame->changeState(std::make_unique<GameState>(mGame));
        } else if (mSelectedOptionIndex == 2) {
          mGame->popState();
          mGame->changeState(std::make_unique<MenuState>(mGame));
        }
      }
    }
  }

  if (const auto *keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPress->code == sf::Keyboard::Key::Escape) {
      mGame->popState();
    } else if (keyPress->code == sf::Keyboard::Key::Up ||
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
      if (mSelectedOptionIndex == 0)
        mGame->popState();
      else if (mSelectedOptionIndex == 1)
        mGame->clearStatesAndPush(std::make_unique<GameState>(mGame));
      else if (mSelectedOptionIndex == 2)
        mGame->clearStatesAndPush(std::make_unique<MenuState>(mGame));
    }
  }
}

void PauseState::update(sf::Time dt) {}

void PauseState::render(sf::RenderWindow &window) {
  window.setView(window.getDefaultView());
  window.draw(mBackground);
  window.draw(mPauseText);
  for (auto &button : mButtons)
    button.render(window);
}
