#include "Game.hpp"
#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode({1280, 720}), "Retro Boy"),
      mCamera(mWindow.getDefaultView()), mPlayer(), mMap() {
  loadLevel("assets/levels/testX.txt");
}

void Game::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;

  while (mWindow.isOpen()) {
    sf::Time dt = clock.restart();
    timeSinceLastUpdate += dt;

    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;

      processEvents();
      update(TimePerFrame);
    }

    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      mWindow.close();
    }

    // Handle Resizing
    if (const auto *resized = event->getIf<sf::Event::Resized>()) {
      sf::Vector2f newSize(static_cast<float>(resized->size.x),
                           static_cast<float>(resized->size.y));
      mCamera.setSize(newSize);
    }

    // Key Presses
    if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPress->code == sf::Keyboard::Key::R) {
        mPlayer.reset(mMap.getStartPosition());
      }
      // Map Switching
      if (keyPress->code == sf::Keyboard::Key::Num1) {
        loadLevel("assets/levels/testX.txt");
      }
      if (keyPress->code == sf::Keyboard::Key::Num2) {
        loadLevel("assets/levels/testY.txt");
      }
      if (keyPress->code == sf::Keyboard::Key::Num3) {
        loadLevel("assets/levels/testFall.txt");
      }
    }
  }
}

void Game::update(sf::Time dt) {
  mPlayer.update(dt.asSeconds(), mMap);

  // Death Logic (Falling off map)
  if (mPlayer.getPosition().y > mMap.getHeight() + 200.f) {
    mPlayer.reset(mMap.getStartPosition());
  }

  // Camera Logic
  sf::Vector2f playerPos = mPlayer.getPosition();
  sf::Vector2f viewSize = mCamera.getSize();
  float camX, camY;

  // CENTER MAP X
  float mapW = mMap.getWidth();
  if (mapW < viewSize.x) {
    camX = mapW / 2.f;
  } else {
    camX = std::max(playerPos.x, viewSize.x / 2.f);
    camX = std::min(camX, mapW - viewSize.x / 2.f);
  }

  // CENTER MAP Y
  float mapH = mMap.getHeight();
  if (mapH < viewSize.y) {
    camY = mapH / 2.f;
  } else {
    camY = std::max(playerPos.y, viewSize.y / 2.f);
    camY = std::min(camY, mapH - viewSize.y / 2.f);
  }

  mCamera.setCenter({camX, camY});
  mWindow.setView(mCamera);
}

void Game::render() {
  mWindow.clear(sf::Color::White);

  mMap.render(mWindow);
  mPlayer.render(mWindow);

  mWindow.display();
}

void Game::loadLevel(const std::string &filename) {
  // Only reload if needed, but for now simple reload
  if (mMap.loadFromFile(filename)) {
    mPlayer.reset(mMap.getStartPosition());
  } else {
    // Fallback or error logging?
    // std::cerr << "Failed to load level: " << filename << std::endl;
  }
}
