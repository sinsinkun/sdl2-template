#pragma once
#include <SDL2/SDL.h>

namespace Global {
  // -- Constants --
  const int WIN_W = 800;
  const int WIN_H = 600;
  const int WIN_MIN_W = 400;
  const int WIN_MIN_H = 300;
  // -- App definition --
  class App {
    public:
      // SDL stuff
      SDL_Renderer* renderer;
      SDL_Window* window;
      // custom global states
      Uint8 bgColor[3] = {20, 20, 30};
      Sint32 mousePos[2] = {0, 0};
      Sint32 winSize[2] = {WIN_W, WIN_H};
      Uint32 deltaTime = 0; // in ms
      Uint32 elapsedTime = 0; // in ms
      float fps = 0;
      // systems
      void init();
      void update();
      void render();
    private:
      // internal timer helper
      Uint32 _alphaTime = 0;
      void _updateTime();
      // internal input handlers
      void _handleInputs();
      void _handleKeyboardEvent(SDL_KeyboardEvent& key);
      void _handleMouseMove(SDL_MouseMotionEvent& move);
      void _handleMouseButton(SDL_MouseButtonEvent& mouse);
      void _handleWindowEvent(SDL_WindowEvent& win);
  };
}
