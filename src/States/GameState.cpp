#include "GameState.hpp"
#include "../Game.hpp"
#include "PauseState.hpp"
#include <iostream>

GameState::GameState(Game *game)
    : State(game), mCamera({0.f, 0.f}, {960.f, 540.f}), mPlayer(), mMap(),
      mBackgroundSprite(mBackgroundTexture), mShowHitbox(false),
      mShowFPS(false), mFrameCount(0), mCurrentFPS(0), mResetTimer(0.f),
      mIsResetting(false), mCurrentLevelIndex(0) {

  mFadeOverlay.setSize({1280, 720}); // Will update in update/render
  mFadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));

  mFPSFontLoaded = mFPSFont.openFromFile("assets/fonts/font.ttf");

  mLevels = {"assets/maps/tutorial.tmx", "assets/maps/level1.tmx"};
  loadLevel(mLevels[mCurrentLevelIndex]);
}

void GameState::loadLevel(const std::string &filename) {
  if (mMap.loadFromFile(filename)) {
    mPlayer.reset(mMap.getStartPosition());
    // Camera setup
    sf::Vector2f playerPos = mPlayer.getPosition();
    sf::Vector2f viewSize = mCamera.getSize();
    float mapW = mMap.getWidth();
    float mapH = mMap.getHeight();

    float camX = std::max(playerPos.x, viewSize.x / 2.f);
    camX = std::min(camX, mapW - viewSize.x / 2.f);
    float camY = std::max(playerPos.y, viewSize.y / 2.f);
    camY = std::min(camY, mapH - viewSize.y / 2.f);
    mCamera.setCenter({camX, camY});

    float bgScale = mBackgroundSprite.getScale().x;
    int texWidth = static_cast<int>((mMap.getWidth() * 1.0f) / bgScale);
    int texHeight = static_cast<int>((mMap.getHeight() * 1.0f) / bgScale);
    mBackgroundSprite.setTextureRect(
        sf::IntRect({0, 0}, {texWidth, texHeight}));
  } else {
    std::cerr << "Failed to load level: " << filename << std::endl;
  }
}

void GameState::handleInput(sf::Event &event) {
  if (const auto *keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPress->code == sf::Keyboard::Key::Escape) {
      mGame->pushState(std::make_unique<PauseState>(mGame));
    }
    if (keyPress->code == sf::Keyboard::Key::F2) {
      mShowFPS = !mShowFPS;
    }
    if (keyPress->code == sf::Keyboard::Key::F1) {
      mShowHitbox = !mShowHitbox;
    }
  }
}

void GameState::update(sf::Time dt) {
  // Smart Reset Logic
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
    mResetTimer += dt.asSeconds();
    float cycleTime = 2.0f; // 1s Fade Out + 1s Fade In

    // Wrap timer for cyclic resetting
    if (mResetTimer >= cycleTime) {
      mResetTimer -= cycleTime;
      mIsResetting = false; // Ready for next reset
    }

    float alpha = 0.f;
    if (mResetTimer < 1.0f) {
      alpha = (mResetTimer / 1.0f) * 255.f;
    } else {
      // Check if we need to trigger reset logic (just once per cycle)
      if (!mIsResetting) {
        mPlayer.reset(mMap.getStartPosition());
        // std::cout << "Smart Reset Triggered!" << std::endl;
        mIsResetting = true;
      }
      alpha = 255.f - ((mResetTimer - 1.0f) / 1.0f) * 255.f;
    }

    // Clamp alpha
    alpha = std::max(0.f, std::min(255.f, alpha));
    mFadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));

  } else {
    // Release R -> Fade back to clear smoothly
    if (mResetTimer > 0.f) {
      if (mResetTimer >= 1.0f && mIsResetting) {
        float currentAlpha = mFadeOverlay.getFillColor().a;
        float fadeSpeed = 500.f; // Alpha per second
        currentAlpha -= fadeSpeed * dt.asSeconds();
        if (currentAlpha < 0.f)
          currentAlpha = 0.f;
        mFadeOverlay.setFillColor(
            sf::Color(0, 0, 0, static_cast<uint8_t>(currentAlpha)));

        // Reset timer logic so next press starts fresh
        mResetTimer = 0.f;
        mIsResetting = false;
      } else {
        // Phase 1 or just starting: Fade back to 0
        mResetTimer -= dt.asSeconds() * 2.0f; // Fast rewind
        if (mResetTimer < 0.f)
          mResetTimer = 0.f;
        float alpha = (mResetTimer / 1.0f) * 255.f;
        mFadeOverlay.setFillColor(
            sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));
        mIsResetting = false;
      }
    } else {
      mFadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
      mIsResetting = false;
    }
  }

  mPlayer.update(dt.asSeconds(), mMap);

  // Death
  if (mPlayer.getPosition().y > mMap.getHeight() + 200.f) {
    mPlayer.reset(mMap.getStartPosition());
  }

  // Finish
  if (mMap.checkFinish(mPlayer.getBounds())) {
    std::cout << "Level Finished!" << std::endl;

    // Check if there is a next level
    if (mCurrentLevelIndex + 1 < mLevels.size()) {
      mCurrentLevelIndex++;
      loadLevel(mLevels[mCurrentLevelIndex]);
    } else {
      std::cout << "Game Completed! Looping back to start." << std::endl;
      mCurrentLevelIndex = 0;
      loadLevel(mLevels[mCurrentLevelIndex]);
    }
  }

  // Camera
  sf::Vector2f playerPos = mPlayer.getPosition();
  sf::Vector2f viewSize = mCamera.getSize();
  sf::Vector2f currentCenter = mCamera.getCenter();

  float mapW = mMap.getWidth();
  float mapH = mMap.getHeight();
  float targetX, targetY;

  if (mapW < viewSize.x)
    targetX = mapW / 2.f;
  else {
    targetX = std::max(playerPos.x, viewSize.x / 2.f);
    targetX = std::min(targetX, mapW - viewSize.x / 2.f);
  }

  if (mapH < viewSize.y)
    targetY = mapH / 2.f;
  else {
    targetY = std::max(playerPos.y, viewSize.y / 2.f);
    targetY = std::min(targetY, mapH - viewSize.y / 2.f);
  }

  float lerpSpeed = 5.0f;
  float newX = currentCenter.x +
               (targetX - currentCenter.x) * lerpSpeed * dt.asSeconds();
  float newY = currentCenter.y +
               (targetY - currentCenter.y) * lerpSpeed * dt.asSeconds();

  mCamera.setCenter({std::round(newX), std::round(newY)});
}

void GameState::render(sf::RenderWindow &window) {
  window.setView(mCamera);

  mMap.render(window, mPlayer.getPosition());
  mPlayer.render(window, mShowHitbox);

  // Draw fade overlay (reset effect) - ensure view is window size
  window.setView(window.getDefaultView());
  mFadeOverlay.setSize(sf::Vector2f(window.getSize())); // Fix fullscreen fade
  window.draw(mFadeOverlay);

  // FPS
  mFrameCount++;
  if (mFPSClock.getElapsedTime().asSeconds() >= 0.1f) {
    mCurrentFPS =
        static_cast<int>(mFrameCount / mFPSClock.getElapsedTime().asSeconds());
    mFrameCount = 0;
    mFPSClock.restart();
  }

  if (mShowFPS && mFPSFontLoaded) {
    window.setView(window.getDefaultView());
    sf::Text fpsText(mFPSFont);
    fpsText.setString(std::to_string(mCurrentFPS));
    fpsText.setCharacterSize(16);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(1.f);
    float textWidth = fpsText.getLocalBounds().size.x;
    fpsText.setPosition({window.getSize().x - textWidth - 10.f, 10.f});
    window.draw(fpsText);
  }
}
