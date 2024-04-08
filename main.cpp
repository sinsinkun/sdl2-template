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
    if (app.winFocus) SDL_Delay(7); // artificial delay
    else SDL_Delay(32);
  }

  // cleanup before exit
  app.cleanup();
  return 0;
}
