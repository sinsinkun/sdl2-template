#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Util {
  std::string floatToString(float n);
  std::string floatToString(float n, int precision);
  void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
}