#include <Game/Game.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/MenuState.hpp>
#include <iostream>

MenuState::MenuState(Game *game)
    : State(game), mSelectedOptionIndex(0),
      mBackgroundSprite(mBackgroundTexture), mBackgroundOffset({0.f, 0.f}) {
  if (!mFont.openFromFile("assets/fonts/font.ttf"))
    std::cerr << "Failed to load font in MenuState!" << std::endl;

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg.png"))
    std::cerr << "Failed to load bg.png in MenuState!" << std::endl;
  mBackgroundTexture.setRepeated(true);
  mBackgroundSprite.setTexture(mBackgroundTexture);

  mButtons.emplace_back(mFont, "Start Game", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Exit", sf::Vector2f{0, 0});
  mButtons[0].select(true);

  mLastWindowSize = mGame->getWindow().getSize();
  updateLayout();
}

void MenuState::updateLayout() {
  sf::Vector2u windowSize = mGame->getWindow().getSize();
  mLastWindowSize = windowSize;

  float centerX = windowSize.x / 2.f;
  float centerY = windowSize.y / 2.f;

  if (mButtons.size() >= 2) {
    mButtons[0].setPosition({centerX, centerY - 50.f});
    mButtons[1].setPosition({centerX, centerY + 50.f});
  }
}

void MenuState::handleInput(sf::Event &event) {
  if (event.is<sf::Event::Resized>())
    updateLayout();

  if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
        {mouseMove->position.x, mouseMove->position.y});

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
          {mouseClick->position.x, mouseClick->position.y});

      if (mButtons[mSelectedOptionIndex].contains(mousePos)) {
        if (mSelectedOptionIndex == 0)
          mGame->changeState(std::make_unique<GameState>(mGame));
        else if (mSelectedOptionIndex == 1)
          const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
      }
    }
  }

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
      if (mSelectedOptionIndex == 0)
        mGame->changeState(std::make_unique<GameState>(mGame));
      else if (mSelectedOptionIndex == 1)
        const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
    }
  }
}

void MenuState::update(sf::Time dt) {
  sf::Vector2u currentSize = mGame->getWindow().getSize();
  if (currentSize != mLastWindowSize)
    updateLayout();

  float speedX = 20.f;
  float speedY = -5.f;
  mBackgroundOffset.x += speedX * dt.asSeconds();
  mBackgroundOffset.y += speedY * dt.asSeconds();
}

void MenuState::render(sf::RenderWindow &window) {
  sf::Vector2f size(static_cast<float>(window.getSize().x),
                    static_cast<float>(window.getSize().y));
  window.setView(sf::View(sf::FloatRect({0, 0}, size)));
  window.clear(sf::Color::Black);

  mBackgroundSprite.setTextureRect(
      sf::IntRect({static_cast<int>(mBackgroundOffset.x),
                   static_cast<int>(mBackgroundOffset.y)},
                  {static_cast<int>(size.x), static_cast<int>(size.y)}));
  window.draw(mBackgroundSprite);

  for (auto &button : mButtons)
    button.render(window);
}
