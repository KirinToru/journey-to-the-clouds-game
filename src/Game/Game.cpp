#include <Game/Game.hpp>
#include <Game/States/MenuState.hpp>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() : mWindow(sf::VideoMode({1280, 720}), "Journey to the Clouds") {
  mWindow.setFramerateLimit(60);
  mWindow.setVerticalSyncEnabled(true);
  mStates.push_back(std::make_unique<MenuState>(this));
}

void Game::pushState(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::Push, std::move(state)});
}

void Game::popState() { mPendingChanges.push_back({Action::Pop, nullptr}); }

void Game::changeState(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::Change, std::move(state)});
}

void Game::clearStatesAndPush(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::ClearAndPush, std::move(state)});
}

void Game::applyPendingChanges() {
  for (auto &change : mPendingChanges) {
    switch (change.action) {
    case Action::Push:
      mStates.push_back(std::move(change.state));
      break;
    case Action::Pop:
      if (!mStates.empty())
        mStates.pop_back();
      break;
    case Action::Change:
      if (!mStates.empty())
        mStates.pop_back();
      mStates.push_back(std::move(change.state));
      break;
    case Action::ClearAndPush:
      mStates.clear();
      mStates.push_back(std::move(change.state));
      break;
    }
  }
  mPendingChanges.clear();
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
      applyPendingChanges();
    }
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>())
      mWindow.close();

    if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPress->code == sf::Keyboard::Key::F4)
        cycleWindowMode();
    }

    if (!mStates.empty()) {
      sf::Event ev = *event;
      mStates.back()->handleInput(ev);
    }
  }
}

void Game::update(sf::Time dt) {
  if (!mStates.empty())
    mStates.back()->update(dt);
}

void Game::render() {
  mWindow.clear(sf::Color::Black);
  for (const auto &state : mStates)
    state->render(mWindow);
  mWindow.display();
}

void Game::cycleWindowMode() {
  mWindowMode = (mWindowMode + 1) % 3;

  switch (mWindowMode) {
  case 0:
    mWindow.create(sf::VideoMode({1280, 720}), "Journey to the Clouds",
                   sf::Style::Default);
    break;
  case 1:
    mWindow.create(sf::VideoMode::getDesktopMode(), "Journey to the Clouds",
                   sf::Style::None);
    break;
  case 2:
    mWindow.create(sf::VideoMode::getDesktopMode(), "Journey to the Clouds",
                   sf::State::Fullscreen);
    break;
  }
  mWindow.setFramerateLimit(60);
  mWindow.setVerticalSyncEnabled(true);
}
