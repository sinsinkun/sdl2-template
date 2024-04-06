#include <iostream>
#include <SDL2/SDL.h>
// #include <GL/gl.h>
// #include <GL/glu.h>

#define WIDTH 800
#define HEIGHT 600

struct App {
  public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    // custom global states
    Uint8 bgColor[3] = {10, 10, 50};
} app;

/// @brief Start SDL window instance
void initSDL() {
  int renderFlags, windowFlags;
  renderFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = 0; // SDL_WINDOW_OPENGL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  // OpenGL setup
  // SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5);
  // SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5);
  // SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5);
  // SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
  // SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

  app.window = SDL_CreateWindow(
    "Game Window",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH,
    HEIGHT,
    windowFlags
  );

  if (!app.window) {
    std::cout << "Failed to open window" << SDL_GetError() << std::endl;
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  app.renderer = SDL_CreateRenderer(app.window, -1, renderFlags);
  // SDL_GLContext context = SDL_GL_CreateContext(app.window);

  if (!app.renderer) {
    std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
    exit(1);
  }
}

/// @brief Reset render space
void clearRender() {
  SDL_SetRenderDrawColor(app.renderer, app.bgColor[0], app.bgColor[1], app.bgColor[2], 255);
  SDL_RenderClear(app.renderer);
}

/// @brief Accept inputs
void takeInput() {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        std::cout << "Pressed key: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
        break;
      case SDL_QUIT:
        exit(0);
        break;
      default:
        break;
    }
  }
}

/// @brief Draw to window
void render() {
  SDL_RenderPresent(app.renderer);
}

/// @brief Entry point
/// @param argc input arguments count
/// @param argv input arguments array
/// @return return value
int main(int argc, char* argv[]) {
  initSDL();

  // event loop
  while (true) {
    clearRender();
    takeInput();
    render();
    SDL_Delay(16); // artificial delay
  }

  return 0;
}
