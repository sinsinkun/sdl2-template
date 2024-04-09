#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Global {
  // -- Constants --
  const int WIN_W = 800;
  const int WIN_H = 600;
  const int WIN_MIN_W = 400;
  const int WIN_MIN_H = 300;

  // -- Helper structs --
  struct TextureCache {
    int x = 0;
    int y = 0;
    SDL_Texture* texture;
  };

  // -- App definition --
  class App {
    public:
      // SDL stuff
      SDL_Renderer* renderer;
      SDL_Window* window;
      TTF_Font* fontp1;
      // custom global states
      bool running = true;
      Uint8 bgColor[3] = {20, 20, 30};
      Sint32 mousePos[2] = {0, 0};
      bool mouseClicking = false;
      Sint32 winSize[2] = {WIN_W, WIN_H};
      bool winFocus = true;
      Uint32 deltaTime = 0; // in ms
      Uint32 elapsedTime = 0; // in ms
      float fps = 0;
      // asset store
      std::vector<TextureCache> textCache;
      // systems
      void init();
      void update();
      void render();
      void cleanup();
    private:
      // timer helpers
      Uint8 _fpsSkip = 0;
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
