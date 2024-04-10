#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "app.h"
#include "util.h"

using namespace Global;

/// @brief Start SDL window instance
void App::init() {
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  // initialize window
  int windowFlags = SDL_WINDOW_RESIZABLE;
  window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, windowFlags);
  if (!window) {
    std::cout << "Failed to open window: " << SDL_GetError() << std::endl;
    exit(1);
  }

  SDL_SetWindowMinimumSize(window, WIN_MIN_W, WIN_MIN_H);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  // initialize renderer
  int renderFlags = SDL_RENDERER_ACCELERATED;
  renderer = SDL_CreateRenderer(window, -1, renderFlags);
  if (!renderer) {
    std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
    exit(1);
  }

  // initialize TTF
  if (TTF_Init() < 0) {
    std::cout << "Failed to load TTF library: " << SDL_GetError() << std::endl;
    exit(1);
  }
  fontp1 = TTF_OpenFont("assets/roboto.ttf", 16);
  if (fontp1 == nullptr) {
    std::cout << "Failed to load font: " << SDL_GetError() << std::endl;
  }
  TTF_SetFontStyle(fontp1, TTF_STYLE_NORMAL);

  // initialize cursor
  SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  SDL_SetCursor(cursor);
}

/// @brief Free resources on exit
void App::cleanup() {
  std::cout << "Closing program (lifetime: " << (float)elapsedTime/1000 << "s)" << std::endl;
  // free cache
  for (TextureCache tc: textCache) {
    SDL_DestroyTexture(tc.texture);
  }
  for (Button b: btnCache) {
    b.destroy();
  }
  // free systems
  TTF_CloseFont(fontp1);
  TTF_Quit();
  SDL_FreeCursor(cursor);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

/// @brief Clean up existing resources and update internal states before update
void App::preupdate() {
  // -- clear old render --
  SDL_SetRenderDrawColor(renderer, bgColor[0], bgColor[1], bgColor[2], 255);
  SDL_RenderClear(renderer);
  // -- update internal states --
  _handleInputs();
  _updateTime();
  _cursorCounter = 0;
}

/// @brief Update app state
void App::update() {
  // -- update internal states --
  _handleInputs();
  _updateTime();
  _cursorCounter = 0;
  // -- logic updates --

  // create cached text
  if (textCache.size() < 1) {
    std::string txt = "Hello cache";
    SDL_Color txtc = {200, 200, 240};
    SDL_Texture* txtcache = Util::createTextCache(renderer, fontp1, txt.c_str(), txtc);
    TextureCache txttc = { winSize[0]/2 - 50, 10, txtcache };
    textCache.push_back(txttc);
  }

  // create button or update
  if (btnCache.size() < 1) {
    TTF_Font* btnf = TTF_OpenFont("assets/roboto.ttf", 18);
    Global::Button btn{ 1, "Button Test" };
    btn.font = btnf;
    btn.pos[0] = 10;
    btn.pos[1] = winSize[1] - 50;
    btn.size[0] = 110;
    btn.size[1] = 40;
    btnCache.push_back(btn);
  } else {
    btnCache.at(0).update(mousePos, mouseClicking, _cursorCounter);
  }

  // update cursor state
  SDL_FreeCursor(cursor);
  if (_cursorCounter > 0) cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
  else cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  SDL_SetCursor(cursor);
}

/// @brief Draw to window
void App::render() {
  // -- clear old render --
  SDL_SetRenderDrawColor(renderer, bgColor[0], bgColor[1], bgColor[2], 255);
  SDL_RenderClear(renderer);
  // -- create new render --

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
  Uint8 g = SDL_clamp(fps * 4 - 20, 0, 255);
  Uint8 r = 255 - g;
  std::string fpsStr = Util::floatToString(fps, 2);
  std::string fpsTxt = "FPS: ";
  fpsTxt.append(fpsStr);
  const char* str = fpsTxt.c_str();
  const SDL_Color fpsColor = {r, g, 80};
  Util::renderText(renderer, fontp1, str, 10, 10, fpsColor);

  // render cached text
  for (TextureCache tc: textCache) {
    Util::renderCachedTexture(renderer, tc.texture, tc.x, tc.y);
  }

  // render btns
  for (Button b: btnCache) {
    b.render(renderer);
  }

  // -- draw new render --
  SDL_RenderPresent(renderer);
}

/// @brief Update timers per frame
void App::_updateTime() {
  Uint32 prevTime = _alphaTime;
  _alphaTime = SDL_GetPerformanceCounter();
  deltaTime = (_alphaTime - prevTime) * 1000 / SDL_GetPerformanceFrequency();
  elapsedTime += deltaTime;
  if (_fpsSkip < 10) { // calculate fps at 10% speed
    _fpsSkip++;
  } else {
    fps = 1000.f / (float)deltaTime;
    _fpsSkip = 0;
  }
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
        // safe exit
        running = false;
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
        running = false;
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
        mouseClicking = true;
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
    mouseClicking = false;
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
      // clear cache
      for (TextureCache tc: textCache) {
        SDL_DestroyTexture(tc.texture);
      }
      textCache.pop_back();
      for (Button b: btnCache) {
        b.destroy();
      }
      btnCache.pop_back();
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      winFocus = false;
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      winFocus = true;
      break;
    default:
      break;
  }
}
#pragma endregion Input_Handling