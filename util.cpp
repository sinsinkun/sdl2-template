#include <iostream>
#include <iomanip>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "util.h"

using namespace Util;

/// @brief convert float to string
/// @param n 
/// @return string form of float with 2 decimal points
std::string Util::floatToString(float n) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << n;
  std::string str = ss.str();
  return str;
};

/// @brief convert float to string
/// @param n 
/// @param precision 
/// @return string form of float with specific precision
std::string Util::floatToString(float n, int precision) {
  // handle precision exceptions
  int p = 8;
  if (precision < p) p = precision;
  else if (precision < 0) p = 0;

  std::stringstream ss;
  ss << std::fixed << std::setprecision(precision) << n;
  std::string str = ss.str();
  return str;
};

/// @brief Render text using SDL renderer
/// @param renderer 
/// @param font 
/// @param text 
/// @param x 
/// @param y 
/// @param color 
void Util::renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
  SDL_Surface* ttfSurface = TTF_RenderUTF8_Blended(font, text, color);
  SDL_Texture* ttfTexture = SDL_CreateTextureFromSurface(renderer, ttfSurface);
  int ttfW = 0, ttfH = 0;
  SDL_QueryTexture(ttfTexture, nullptr, nullptr, &ttfW, &ttfH);
  SDL_Rect dstrect = {x, y, ttfW, ttfH};
  SDL_RenderCopy(renderer, ttfTexture, nullptr, &dstrect);

  // clean up (TODO: optimize by caching)
  SDL_FreeSurface(ttfSurface);
  SDL_DestroyTexture(ttfTexture);
}

/// @brief Create cache of text texture for re-use
/// @param renderer 
/// @param font 
/// @param text 
/// @param color 
/// @return SDL_Texture* texture
SDL_Texture* Util::createTextCache(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
  SDL_Surface* ttfSurface = TTF_RenderUTF8_Blended(font, text, color);
  SDL_Texture* ttfTexture = SDL_CreateTextureFromSurface(renderer, ttfSurface);
  
  // clean up
  SDL_FreeSurface(ttfSurface);
  return ttfTexture;
}

/// @brief Render texture from cache
/// @param renderer 
/// @param cache 
/// @param x 
/// @param y 
void Util::renderCachedTexture(SDL_Renderer* renderer, SDL_Texture* cache, int x, int y) {
  int w = 0, h = 0;
  SDL_QueryTexture(cache, nullptr, nullptr, &w, &h);
  SDL_Rect dstrect = {x, y, w, h};
  SDL_RenderCopy(renderer, cache, nullptr, &dstrect);
}
