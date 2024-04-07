#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"
#include "../util.h"

using namespace Global;

/// @brief Update button state
/// @param mousePos 
/// @param mouseClick 
void Button::update(Sint32 mousePos[2], bool mouseClick) {
  bool hovered = _isHovered(mousePos);
  bool newClick = hovered && mouseClick;
  // set state
  if (hovered && !newClick && !_clicking) state = ButtonState::Hover;
  else if (newClick && !_clicking) state = ButtonState::Just_Clicked;
  else if (newClick && _clicking) state = ButtonState::Clicking;
  else if (!newClick && _clicking) state = ButtonState::Just_Released;
  else state = ButtonState::None;

}

/// @brief Render button to renderer
/// @param renderer 
void Button::render(SDL_Renderer* renderer) {
  // draw background
  SDL_Rect rect = {pos[0], pos[1], size[0], size[1]};
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
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, bgColor.a);
  SDL_RenderFillRect(renderer, &rect);
  //draw text
  if (text.length() > 0 && font != nullptr) {
    const char* str = text.c_str();
    Util::renderText(renderer, font, str, pos[0] + 10, pos[1] + 10, textColor);
  }
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
