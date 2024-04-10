#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Global {

  enum ButtonState {
    None,
    Hover,
    Just_Clicked,
    Clicking,
    Just_Released,
  };

  class Button {
    public:
      // initialization
      Button(int id): id(id) { };
      Button(int id, std::string text): id(id), text(text) { };
      // state
      int id = -1;
      Sint32 pos[2] = {0, 0};
      Sint32 size[2] = {40, 20};
      SDL_Color bgColor = {180, 180, 180};
      SDL_Color hoverColor = {180, 180, 200};
      SDL_Color clickColor = {160, 160, 160};
      SDL_Color textColor = {0, 0, 0};
      std::string text;
      TTF_Font* font = nullptr;
      ButtonState state = ButtonState::None;
      // methods
      void update(Sint32 mousePos[2], bool mouseClick, int& cursorCounter);
      void render(SDL_Renderer* renderer);
      void destroy();
    private:
      bool _clicking = false;
      bool _isHovered(Sint32 mousePos[2]);
  };
}