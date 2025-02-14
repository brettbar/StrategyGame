#pragma once

#include "../shared/common.hpp"
#include "clay/clay.h"


namespace UI {
  const float UI_SCALE = 2.0;

  const Clay_Color COLOR_BLACK = Clay_Color{ 0, 0, 0, 255 };
  const Clay_Color COLOR_WHITE = Clay_Color{ 255, 255, 255, 255 };
  const Clay_Color COLOR_SLATE = Clay_Color{ 41, 41, 41, 255 };
  const Clay_Color COLOR_LIGHT = Clay_Color{ 224, 215, 210, 255 };
  const Clay_Color COLOR_RED = Clay_Color{ 205, 66, 66, 255 };
  const Clay_Color COLOR_GREEN = Clay_Color{ 66, 205, 66, 255 };
  const Clay_Color COLOR_BLUE = Clay_Color{ 66, 66, 205, 255 };
  const Clay_Color COLOR_CYAN = Clay_Color{ 0, 255, 255, 255 };
  const Clay_Color COLOR_ORANGE = Clay_Color{ 225, 138, 50, 255 };

  inline void spacer() {
    CLAY( {
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
        },
    } );
  }

  inline Clay_String str_to_cs( str input ) {
    return Clay_String{
      .length = static_cast<int32_t>( strlen( input.c_str() ) ),
      .chars = input.c_str(),
    };
  }
};// namespace UI
