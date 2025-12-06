#pragma once

#include "../../shared/common.hpp"
#include "../common.h"

namespace UI {
  inline void text_label(Clay_String cs, f32 font_size) {
    CLAY_TEXT(
      cs,
      CLAY_TEXT_CONFIG( {
        .textColor = COLOR_WHITE,
        .fontId = 0,
        .fontSize = uint16_t(font_size * UI_SCALE),
      } )
    );
  }
}
