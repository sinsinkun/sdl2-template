#include <SDL2/SDL.h>
#include "app.h"

using namespace Global;

/// @brief Entry point
/// @param argc input arguments count
/// @param argv input arguments array
/// @return return value
int main(int argc, char* argv[]) {
  // declare new app
  App app;
  app.init();

  // event loop
  while (app.running) {
    app.update();
    app.render();
    SDL_Delay(16); // artificial delay
  }

  // cleanup before exit
  app.cleanup();
  return 0;
}
