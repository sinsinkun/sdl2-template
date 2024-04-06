#include <iostream>
#include <SDL2/SDL.h>
// #include <GL/gl.h>
// #include <GL/glu.h>

#define WIDTH 800
#define HEIGHT 600

class App {
  public:
    // SDL stuff
    SDL_Renderer* renderer;
    SDL_Window* window;
    // custom global states
    Uint8 bgColor[3] = {10, 10, 50};
    // systems
    void init();
    void clearRender();
    void update();
    void handleKeyboardEvent(SDL_KeyboardEvent& key);
    void handleWindowEvent(SDL_WindowEvent& win);
    void render();
};

/// @brief Start SDL window instance
void App::init() {
  int renderFlags, windowFlags;
  renderFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = SDL_WINDOW_RESIZABLE; // SDL_WINDOW_OPENGL;

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

  window = SDL_CreateWindow(
    "Game Window",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH,
    HEIGHT,
    windowFlags
  );
  if (!window) {
    std::cout << "Failed to open window" << SDL_GetError() << std::endl;
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  renderer = SDL_CreateRenderer(window, -1, renderFlags);
  // SDL_GLContext context = SDL_GL_CreateContext(app.window);

  if (!renderer) {
    std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
    exit(1);
  }
}

/// @brief Reset render space
void App::clearRender() {
  SDL_SetRenderDrawColor(renderer, bgColor[0], bgColor[1], bgColor[2], 255);
  SDL_RenderClear(renderer);
}

/// @brief Accept inputs
void App::update() {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        handleKeyboardEvent(event.key);
        break;
      case SDL_WINDOWEVENT:
        handleWindowEvent(event.window);
        break;
      case SDL_QUIT:
        exit(0);
        break;
      default:
        break;
    }
  }
}

/// @brief Handle keyboard inputs
/// @param key
void App::handleKeyboardEvent(SDL_KeyboardEvent& key) {
  if (key.type == SDL_KEYDOWN && !key.repeat) {
    switch (key.keysym.sym) {
      case SDLK_ESCAPE:
        // do nothing
        break;
      default:
        std::cout << "Pressed key: " << SDL_GetKeyName(key.keysym.sym) << std::endl;
        break;
    }
  } else if (key.type == SDL_KEYUP) {
    switch (key.keysym.sym) {
      case SDLK_ESCAPE:
        exit(0);
        break;
      default:
        std::cout << "Let go of key: " << SDL_GetKeyName(key.keysym.sym) << std::endl;
        break;
    }
  }
}

/// @brief Handle window events
/// @param win 
void App::handleWindowEvent(SDL_WindowEvent& win) {
  switch (win.event) {
    // case SDL_WINDOWEVENT_RESIZED: <-- happens after size_changed event
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      break;
    case SDL_WINDOWEVENT_HIDDEN:
    case SDL_WINDOWEVENT_SHOWN:
    default:
      break;
  }
}

/// @brief Draw to window
void App::render() {
  SDL_RenderPresent(renderer);
}

/// @brief Entry point
/// @param argc input arguments count
/// @param argv input arguments array
/// @return return value
int main(int argc, char* argv[]) {
  // declare new app
  App app;
  app.init();

  // event loop
  while (true) {
    app.clearRender();
    app.update();
    app.render();
    SDL_Delay(16); // artificial delay
  }

  return 0;
}
