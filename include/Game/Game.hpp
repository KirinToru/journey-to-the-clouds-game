#pragma once

#include <Engine/States/State.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>

class Game {
public:
  Game();
  void run();

  void pushState(std::unique_ptr<State> state);
  void popState();
  void changeState(std::unique_ptr<State> state);

  // Clear all states and push a new one (e.g. Restart/Menu)
  void clearStatesAndPush(std::unique_ptr<State> state);

  const sf::RenderWindow &getWindow() const { return mWindow; }

  void cycleWindowMode();

private:
  void processEvents();
  void update(sf::Time dt);
  void render();

  void applyPendingChanges();

  sf::RenderWindow mWindow;
  std::vector<std::unique_ptr<State>> mStates;

  static const sf::Time TimePerFrame;

  int mWindowMode = 0; // 0=windowed, 1=maximized, 2=fullscreen

  // Pending State Changes
  enum class Action {
    Push,
    Pop,
    Change,
    ClearAndPush // Changes stack to [NewState]
  };

  struct PendingChange {
    Action action;
    std::unique_ptr<State> state;
  };

  std::vector<PendingChange> mPendingChanges;
};
