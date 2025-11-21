#include "Game.hpp"
#include <SDL3/SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void main_loop(void *arg) {
  Game *game = static_cast<Game *>(arg);
  game->loop();
}

int main(int argc, char *argv[]) {
  Game game;

  if (game.init("Yaka - Pong", 800, 600)) {

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, &game, -1, 1);
#else
    game.run();
#endif
  }
  return 0;
}
