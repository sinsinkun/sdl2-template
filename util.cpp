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

  // clean up (unoptimized, do not run every frame)
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

/// @brief Draw circle
/// @param renderer 
/// @param x 
/// @param y 
/// @param radius 
/// @param color 
void Util::renderCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
      }
    }
  }
}

/// @brief Draw circle using midpoint circle algorithm (4x faster)
/// @param renderer 
/// @param x 
/// @param y 
/// @param r 
/// @param color 
void Util::renderCircleFast(SDL_Renderer* renderer, int x, int y, int r, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  int t1 = r / 16, dx = r, dy = 0;
  while (dx > dy) {
    SDL_RenderDrawLine(renderer, x + dx, y + dy, x - dx, y + dy);
    SDL_RenderDrawLine(renderer, x + dx, y - dy, x - dx, y - dy);
    SDL_RenderDrawLine(renderer, x + dy, y + dx, x - dy, y + dx);
    SDL_RenderDrawLine(renderer, x + dy, y - dx, x - dy, y - dx);
    dy++;
    t1 = t1 + dy;
    if (t1 - dx  >= 0) {
      t1 = t1 - dx;
      dx = dx - 1;
    }
  }
}

/// @brief Draw donut
/// @param renderer 
/// @param x 
/// @param y 
/// @param radius 
/// @param innerRadius 
/// @param color 
void Util::renderDonut(SDL_Renderer *renderer, int x, int y, int radius, int innerRadius, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius) && (dx*dx + dy*dy) >= (innerRadius * innerRadius)) {
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
      }
    }
  }
}

/// @brief Draw rectangle with rounded corners
/// @param renderer 
/// @param x 
/// @param y 
/// @param w
/// @param h
/// @param cradius radius of corner
/// @param color 
void Util::renderRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h, int cr, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  // handle radius that's too big
  if (cr > w/2) cr = w/2;
  if (cr > h/2) cr = h/2;
  // draw pixels
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      int di = -1, dj = -1;
      // calculate distance from respective corner
      if (i < cr) di = cr - i;
      else if (i > w-cr) di = w - cr - i;
      if (j < cr) dj = cr - j;
      else if (j > h-cr) dj = h - cr - j;
      // draw pixel
      if (di == -1 && dj == -1) SDL_RenderDrawPoint(renderer, x+i, y+j);
      else if ((di*di) + (dj*dj) <= (cr*cr)) SDL_RenderDrawPoint(renderer, x+i, y+j);
    }
  }
}
