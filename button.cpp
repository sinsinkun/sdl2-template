#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"
#include "util.h"

using namespace Global;

/// @brief Update button state
/// @param mousePos 
/// @param mouseClick 
void Button::update(Sint32 mousePos[2], bool mouseClick, int& cursorCounter) {
  bool hovered = _isHovered(mousePos);
  bool newClick = hovered && mouseClick;
  // set state
  if (hovered && !newClick && !_clicking) state = ButtonState::Hover;
  else if (newClick && !_clicking) {
    state = ButtonState::Just_Clicked;
    _clicking = true;
    pos[0] -= 2;
    pos[1] -= 2;
    size[0] += 4;
    size[1] += 4;
    textOffset[0] += 2;
    textOffset[1] += 2;
  }
  else if (newClick && _clicking) state = ButtonState::Clicking;
  else if (!newClick && _clicking) {
    state = ButtonState::Just_Released;
    _clicking = false;
    pos[0] += 2;
    pos[1] += 2;
    size[0] -= 4;
    size[1] -= 4;
    textOffset[0] -= 2;
    textOffset[1] -= 2;
  }
  else state = ButtonState::None;
  // set cursor
  if (hovered) cursorCounter++;
}

/// @brief Render button to renderer
/// @param renderer 
void Button::render(SDL_Renderer* renderer) {
  // draw background
  SDL_Color color = bgColor;
  switch (state) {
    case ButtonState::Hover:
    case ButtonState::Just_Released:
      color = hoverColor;
      break;
    case ButtonState::Just_Clicked:
    case ButtonState::Clicking:
      color = clickColor;
      break;
    case ButtonState::None:
    default:
      break;
  }
  Util::renderRoundedRect(renderer, pos[0], pos[1], size[0], size[1], cradius, color);

  // cache text
  if (textCache == nullptr && font != nullptr && text.length() > 0) {
    textCache = Util::createTextCache(renderer, font, text.c_str(), textColor);
  }
  //draw text
  if (textCache) {
    Util::renderCachedTexture(renderer, textCache, pos[0] + textOffset[0], pos[1] + textOffset[1]);
  }
}

/// @brief Free resources taken by button
void Button::destroy() {
  if (textCache) SDL_DestroyTexture(textCache);
  TTF_CloseFont(font);
}

/// @brief Check is mouse is hovering over button
/// @param mousePos 
/// @return is hovered
bool Button::_isHovered(Sint32 mousePos[2]) {
  bool xh = false, yh = false;
  if (mousePos[0] > pos[0] && mousePos[0] < pos[0] + size[0]) xh = true;
  if (mousePos[1] > pos[1] && mousePos[1] < pos[1] + size[1]) yh = true;
  if (xh && yh) return true;
  return false;
}
