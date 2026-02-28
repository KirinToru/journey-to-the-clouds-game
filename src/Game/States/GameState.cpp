#include <Game/Game.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/PauseState.hpp>
#include <iostream>

GameState::GameState(Game *game)
    : State(game), mCamera({0.f, 0.f}, {960.f, 540.f}), mPlayer(), mMap(),
      mBackgroundSprite(mBackgroundTexture), mShowHitbox(false),
      mShowFPS(false), mFrameCount(0), mCurrentFPS(0), mResetTimer(0.f),
      mIsResetting(false), mCurrentLevelIndex(0) {

  mFadeOverlay.setSize({1280, 720});
  mFadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg.png"))
    std::cerr << "Failed to load bg.png" << std::endl;
  mBackgroundTexture.setRepeated(true);
  mBackgroundSprite.setTexture(mBackgroundTexture);

  mFPSFontLoaded = mFPSFont.openFromFile("assets/fonts/font.ttf");

  mLevels = {"assets/maps/test.tmx"};
  loadLevel(mLevels[mCurrentLevelIndex]);
}

void GameState::loadLevel(const std::string &filename) {
  if (mMap.loadFromFile(filename)) {
    mPlayer.reset(mMap.getStartPosition());
    sf::Vector2f playerPos = mPlayer.getPosition();
    sf::Vector2f viewSize = mCamera.getSize();
    float mapW = mMap.getWidth();
    float mapH = mMap.getHeight();

    float camX =
        std::clamp(playerPos.x, viewSize.x / 2.f, mapW - viewSize.x / 2.f);
    float camY =
        std::clamp(playerPos.y, viewSize.y / 2.f, mapH - viewSize.y / 2.f);
    mCamera.setCenter({camX, camY});
  } else {
    std::cerr << "Failed to load level: " << filename << std::endl;
  }
}

void GameState::handleInput(sf::Event &event) {
  if (const auto *keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPress->code == sf::Keyboard::Key::Escape)
      mGame->pushState(std::make_unique<PauseState>(mGame));
    if (keyPress->code == sf::Keyboard::Key::F2)
      mShowFPS = !mShowFPS;
    if (keyPress->code == sf::Keyboard::Key::F1)
      mShowHitbox = !mShowHitbox;
  }
}

void GameState::update(sf::Time dt) {
  // Smart Reset Logic
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
    mResetTimer += dt.asSeconds();
    float cycleTime = 2.0f;

    if (mResetTimer >= cycleTime) {
      mResetTimer -= cycleTime;
      mIsResetting = false;
    }

    float alpha = 0.f;
    if (mResetTimer < 1.0f) {
      alpha = (mResetTimer / 1.0f) * 255.f;
    } else {
      if (!mIsResetting) {
        mPlayer.reset(mMap.getStartPosition());
        mIsResetting = true;
      }
      alpha = 255.f - ((mResetTimer - 1.0f) / 1.0f) * 255.f;
    }

    alpha = std::clamp(alpha, 0.f, 255.f);
    mFadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));
  } else {
    if (mResetTimer > 0.f) {
      if (mResetTimer >= 1.0f && mIsResetting) {
        float currentAlpha = mFadeOverlay.getFillColor().a;
        float fadeSpeed = 500.f;
        currentAlpha -= fadeSpeed * dt.asSeconds();
        if (currentAlpha < 0.f)
          currentAlpha = 0.f;
        mFadeOverlay.setFillColor(
            sf::Color(0, 0, 0, static_cast<uint8_t>(currentAlpha)));
        mResetTimer = 0.f;
        mIsResetting = false;
      } else {
        mResetTimer -= dt.asSeconds() * 2.0f;
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

  // Death from falling below map
  if (mPlayer.getPosition().y > mMap.getHeight() + 200.f)
    mPlayer.reset(mMap.getStartPosition());

  // Death from spikes
  if (mMap.checkSpikeCollision(mPlayer.getBounds()))
    mPlayer.reset(mMap.getStartPosition());

  // Finish
  if (mMap.checkFinish(mPlayer.getBounds())) {
    std::cout << "Level Finished!" << std::endl;
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

  float targetX =
      (mapW < viewSize.x)
          ? mapW / 2.f
          : std::clamp(playerPos.x, viewSize.x / 2.f, mapW - viewSize.x / 2.f);
  float targetY =
      (mapH < viewSize.y)
          ? mapH / 2.f
          : std::clamp(playerPos.y, viewSize.y / 2.f, mapH - viewSize.y / 2.f);

  float lerpSpeed = 5.0f;
  float newX = currentCenter.x +
               (targetX - currentCenter.x) * lerpSpeed * dt.asSeconds();
  float newY = currentCenter.y +
               (targetY - currentCenter.y) * lerpSpeed * dt.asSeconds();
  mCamera.setCenter({std::round(newX), std::round(newY)});
}

void GameState::render(sf::RenderWindow &window) {
  window.setView(mCamera);

  // Parallax Background
  sf::Vector2f cameraCenter = mCamera.getCenter();
  sf::Vector2f viewSize = mCamera.getSize();

  mBackgroundSprite.setPosition(
      {cameraCenter.x - viewSize.x / 2.f, cameraCenter.y - viewSize.y / 2.f});

  float parallaxFactorX = 0.2f;
  float parallaxFactorY = 0.1f;

  int texX = static_cast<int>(cameraCenter.x * parallaxFactorX);
  int texY = static_cast<int>(cameraCenter.y * parallaxFactorY);
  mBackgroundSprite.setTextureRect(
      sf::IntRect({texX, texY}, {static_cast<int>(viewSize.x) + 2,
                                 static_cast<int>(viewSize.y) + 2}));

  window.draw(mBackgroundSprite);
  mMap.render(window, mPlayer.getPosition(), mShowHitbox);
  mPlayer.render(window, mShowHitbox);

  // Fade overlay
  window.setView(window.getDefaultView());
  mFadeOverlay.setSize(sf::Vector2f(window.getSize()));
  window.draw(mFadeOverlay);

  // FPS counter
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
