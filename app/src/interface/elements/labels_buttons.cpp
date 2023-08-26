#pragma once

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  void TextLabelElement::Draw( Color background, rect transform )
  {
    // Doesnt everything do this?
    DrawRectangleV(
      { transform.x, transform.y },
      { transform.width, transform.height },
      background
    );

    DrawTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      text.c_str(),
      {
        transform.x,
        transform.y,
      },
      font_size,
      2.0,
      text_color
    );
  }
};// namespace UI
