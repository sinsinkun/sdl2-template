#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include "app.h"

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

using namespace Global;

/// @brief Start SDL window instance
void App::init() {
  // int renderFlags, windowFlags;
  // renderFlags = SDL_RENDERER_ACCELERATED;
  int windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, windowFlags);
  if (!window) {
    std::cout << "Failed to open window" << SDL_GetError() << std::endl;
    exit(1);
  }
  SDL_SetWindowMinimumSize(window, WIN_MIN_W, WIN_MIN_H);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  
  // OpenGL setup
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    std::cout << "Failed to create OpenGL context" << SDL_GetError() << std::endl;
    exit(1);
  }

  // renderer = SDL_CreateRenderer(window, -1, renderFlags);
  // if (!renderer) {
  //   std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
  //   exit(1);
  // }
}

/// @brief Update app state
void App::update() {
  _handleInputs();
  // TODO: logic updates
}

/// @brief Draw to window
void App::render() {
  // -- clear front buffer --
  glViewport(0, 0, winSize[0], winSize[1]);
  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // -- update back buffer --
  // draw triangle (boundaries are -1.0 to 1.0)
  glBegin(GL_TRIANGLES);
    glColor3f(0.8, 0.5, 0.2);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
  glEnd();

  // -- swap front and back --
  SDL_GL_SwapWindow(window);
}

#pragma region Input_Handling
/// @brief Accept inputs
void App::_handleInputs() {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        _handleKeyboardEvent(event.key);
        break;
      case SDL_MOUSEMOTION:
        _handleMouseMove(event.motion);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        _handleMouseButton(event.button);
        break;
      case SDL_WINDOWEVENT:
        _handleWindowEvent(event.window);
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
void App::_handleKeyboardEvent(SDL_KeyboardEvent& key) {
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

/// @brief Handle mouse button clicks
/// @param mouse 
void App::_handleMouseButton(SDL_MouseButtonEvent& mouse) {
  if (mouse.type == SDL_MOUSEBUTTONDOWN) {
    switch (mouse.button) {
      case SDL_BUTTON_LEFT:
        std::cout << "Pressed LMB at (" << mousePos[0] << "," << mousePos[1] << ")" << std::endl;
        break;
      case SDL_BUTTON_RIGHT:
        std::cout << "Pressed RMB" << std::endl;
        break;
      case SDL_BUTTON_MIDDLE:
        std::cout << "Pressed MMB" << std::endl;
        break;
      case SDL_BUTTON_X1:
        std::cout << "Pressed MB X1" << std::endl;
        break;
      case SDL_BUTTON_X2:
        std::cout << "Pressed MB X2" << std::endl;
        break;
      default:
        break;
    }
  }
  else if (mouse.type == SDL_MOUSEBUTTONUP) {
    std::cout << "MB released" << std::endl;
  }
}

/// @brief Handle mouse movement
/// @param move 
void App::_handleMouseMove(SDL_MouseMotionEvent& move) {
  mousePos[0] = move.x;
  mousePos[1] = move.y;
}

/// @brief Handle window events
/// @param win 
void App::_handleWindowEvent(SDL_WindowEvent& win) {
  switch (win.event) {
    // case SDL_WINDOWEVENT_RESIZED: <-- happens after size_changed event
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      winSize[0] = win.data1;
      winSize[1] = win.data2;
      break;
    case SDL_WINDOWEVENT_HIDDEN:
    case SDL_WINDOWEVENT_SHOWN:
    default:
      break;
  }
}
#pragma endregion Input_Handling
