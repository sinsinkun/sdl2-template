#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Util {
  std::string floatToString(float n);
  std::string floatToString(float n, int precision);
  void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
  SDL_Texture* createTextCache(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
  void renderCachedTexture(SDL_Renderer* renderer, SDL_Texture* cache, int x, int y);
  void renderCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color);
  void renderDonut(SDL_Renderer *renderer, int x, int y, int radius, int innerRadius, SDL_Color color);
  void renderRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h, int cradius, SDL_Color color);
}