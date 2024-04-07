#include <iostream>
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "app.h"
#include "util.h"

using namespace Global;

/// @brief Start SDL window instance
void App::init() {
  int renderFlags, windowFlags;
  renderFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = SDL_WINDOW_RESIZABLE;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  // initialize window
  window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, windowFlags);
  if (!window) {
    std::cout << "Failed to open window: " << SDL_GetError() << std::endl;
    exit(1);
  }

  SDL_SetWindowMinimumSize(window, WIN_MIN_W, WIN_MIN_H);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  // initialize renderer
  renderer = SDL_CreateRenderer(window, -1, renderFlags);
  if (!renderer) {
    std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
    exit(1);
  }

  // initialize font
  if (TTF_Init() < 0) {
    std::cout << "Failed to initialize TTF: " << SDL_GetError() << std::endl;
    exit(2);
  }
  font = TTF_OpenFont("assets/retro_computer.ttf", 16);
  if (font == nullptr) {
    std::cout << "Couldn't find font: " << SDL_GetError() << std::endl;
  }
  TTF_SetFontStyle(font, 0);
  // TTF_SetFontOutline(font, 0);
  // TTF_SetFontKerning(font, 0);
  // TTF_SetFontHinting(font, 0);

}

/// @brief Destroy resources on exit
void App::cleanup() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

/// @brief Update app state
void App::update() {
  _handleInputs();
  _updateTime();
  // TODO: logic updates
}

/// @brief Draw to window
void App::render() {
  // -- clear old render --
  SDL_SetRenderDrawColor(renderer, bgColor[0], bgColor[1], bgColor[2], 255);
  SDL_RenderClear(renderer);
  // -- create new render --

  // FPS rect
  // Uint8 g = SDL_clamp(fps * 4 - 80, 0, 255);
  // Uint8 r = 255 - g;
  // SDL_Rect fpsRect = {10, 10, 40, 10};
  // SDL_SetRenderDrawColor(renderer, r, g, 20, 255);
  // SDL_RenderFillRect(renderer, &fpsRect);

  // draw rectangles
  SDL_Rect rect1 = {winSize[0]/2 - 40, winSize[1]/2 - 40, 50, 50};
  SDL_Rect rect2 = {winSize[0]/2 - 25, winSize[1]/2 - 25, 50, 50};
  SDL_Rect rect3 = {winSize[0]/2 - 10, winSize[1]/2 - 10, 50, 50};
  SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255);
  SDL_RenderFillRect(renderer, &rect1);
  SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
  SDL_RenderFillRect(renderer, &rect2);
  SDL_SetRenderDrawColor(renderer, 220, 220, 255, 255);
  SDL_RenderFillRect(renderer, &rect3);

  // draw polygon
  const std::vector<SDL_Vertex> verts = {
    { SDL_FPoint{ 100, 100 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 } },
    { SDL_FPoint{ 60, 250 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 } },
    { SDL_FPoint{ 290, 250 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 } },
    { SDL_FPoint{ 250, 100 }, SDL_Color{ 255, 255, 0, 255 }, SDL_FPoint{ 0 } },
  };
  const int order[] = {0,1,2,0,2,3};
  SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), order, 6);

  // render FPS
  std::string fpsStr = Util::floatToString(fps);
  std::string fpsTxt = "FPS: ";
  fpsTxt.append(fpsStr);
  const char* str = fpsTxt.c_str();
  renderText(str, 10, 10);

  // -- draw new render --
  SDL_RenderPresent(renderer);
}

/// @brief Helper function for rendering text with SDL2_ttf
/// @param text char array for text
/// @param x top left pos.x
/// @param y top left pos.y
void App::renderText(const char* text, int x, int y) {
  const SDL_Color ttfColor = {255, 255, 255, 255};
  SDL_Surface* ttfSurface = TTF_RenderText_Solid(font, text, ttfColor);
  SDL_Texture* ttfTexture = SDL_CreateTextureFromSurface(renderer, ttfSurface);
  int ttfW = 0, ttfH = 0;
  SDL_QueryTexture(ttfTexture, nullptr, nullptr, &ttfW, &ttfH);
  SDL_Rect dstrect = {x, y, ttfW, ttfH};
  SDL_RenderCopy(renderer, ttfTexture, nullptr, &dstrect);
}

/// @brief Update timers per frame
void App::_updateTime() {
  Uint32 prevTime = _alphaTime;
  _alphaTime = SDL_GetPerformanceCounter();
  deltaTime = (_alphaTime - prevTime) * 1000 / SDL_GetPerformanceFrequency();
  elapsedTime += deltaTime;
  fps = 1000 / deltaTime;
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