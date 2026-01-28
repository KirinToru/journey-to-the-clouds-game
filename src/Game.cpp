#include "Game.hpp"
#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : mWindow(sf::VideoMode({1280, 720}), "Journey to the Clouds"),
      mCamera({0.f, 0.f}, {640.f, 360.f}), mPlayer(), mMap(),
      mBackgroundSprite(mBackgroundTexture) {

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg_forest.png")) {
    std::cerr << "Failed to load bg_forest.png" << std::endl;
  }
  mBackgroundSprite.setTexture(mBackgroundTexture, true);

  loadLevel("assets/levels/tutorial.csv");
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
      // Maintain 2x Zoom
      mCamera.setSize({newSize.x / 2.f, newSize.y / 2.f});
    }

    // Key Presses
    if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPress->code == sf::Keyboard::Key::R) {
        mPlayer.reset(mMap.getStartPosition());
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

  // Finish Logic
  if (mMap.checkFinish(mPlayer.getBounds())) {
    std::cout << "Level Finished! Resetting..." << std::endl;
    mPlayer.reset(mMap.getStartPosition());
  }

  // Camera Logic
  sf::Vector2f playerPos = mPlayer.getPosition();
  sf::Vector2f viewSize = mCamera.getSize();
  sf::Vector2f currentCenter = mCamera.getCenter();

  float targetX, targetY;

  // Calculate TARGET X (Clamped to map)
  float mapW = mMap.getWidth();
  if (mapW < viewSize.x) {
    targetX = mapW / 2.f;
  } else {
    targetX = std::max(playerPos.x, viewSize.x / 2.f);
    targetX = std::min(targetX, mapW - viewSize.x / 2.f);
  }

  // Calculate TARGET Y (Clamped to map)
  float mapH = mMap.getHeight();
  if (mapH < viewSize.y) {
    targetY = mapH / 2.f;
  } else {
    targetY = std::max(playerPos.y, viewSize.y / 2.f);
    targetY = std::min(targetY, mapH - viewSize.y / 2.f);
  }

  // Smoothly interpolate current center towards target
  // Factor 5.0f determines the "tightness" of the rubber band
  float lerpSpeed = 5.0f;
  float newX = currentCenter.x +
               (targetX - currentCenter.x) * lerpSpeed * dt.asSeconds();
  float newY = currentCenter.y +
               (targetY - currentCenter.y) * lerpSpeed * dt.asSeconds();

  mCamera.setCenter({newX, newY});
  mWindow.setView(mCamera);
}

void Game::render() {
  mWindow.clear(sf::Color::White);

  // Draw background static to camera
  mWindow.setView(mWindow.getDefaultView());
  mWindow.draw(mBackgroundSprite);

  // Restore World View
  mWindow.setView(mCamera);

  mMap.render(mWindow);
  mPlayer.render(mWindow);

  mWindow.display();
}

void Game::loadLevel(const std::string &filename) {
  // Only reload if needed, but for now simple reload
  if (mMap.loadFromFile(filename)) {
    mPlayer.reset(mMap.getStartPosition());
  } else {
    std::cerr << "Failed to load level: " << filename << std::endl;
  }
}
