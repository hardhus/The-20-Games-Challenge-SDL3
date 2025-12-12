#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"

enum class GameState { MENU, PLAYING };

class Game {
public:
  Game() = default;

  ~Game();
  bool init(const char *title, int width, int height);
  void run();
  void loop();

private:
  void processEvents();
  void update(float dt);
  void render();

  void checkCollisions();
  void resetScores();

  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;

  GameState gameState_ = GameState::MENU;

  bool isRunning_ = false;
  bool isAIActive_ = true;

  int screenWidth_ = 0;
  int screenHeight_ = 0;

  Paddle player1_;
  Paddle player2_;
  Ball ball_;

  int score1_ = 0;
  int score2_ = 0;

  Uint64 lastTick_ = 0;
};
