#pragma once

#include "../shared/common.hpp"
#include "clay/clay.h"
#include <raylib.h>

namespace UI {
  inline float UI_SCALE = 2.0f;

  inline void set_ui_scale(f32 height_dpi) {
    f32 base_dpi = 96.0f;

    UI_SCALE = height_dpi / base_dpi;

    if (UI_SCALE < 2.0f)
      UI_SCALE = 2.0f;
  }

  // inline void set_ui_scale(f32 screen_diagonal_inches) {
  //   // @todo find something that works for these
  //   if (screen_diagonal_inches > 22) {
  //     UI_SCALE = 4.0f;
  //   } else if (screen_diagonal_inches > 14) {
  //     UI_SCALE = 2.0f;
  //   } else {
  //     UI_SCALE = 1.0f;
  //   }
  // }


  const Clay_Color COLOR_TRANSPARENT_BLACK = Clay_Color{0, 0, 0, 200};
  const Clay_Color COLOR_BLACK = Clay_Color{0, 0, 0, 255};
  const Clay_Color COLOR_WHITE = Clay_Color{255, 255, 255, 255};
  const Clay_Color COLOR_SLATE = Clay_Color{41, 41, 41, 255};
  const Clay_Color COLOR_LIGHT = Clay_Color{224, 215, 210, 255};
  const Clay_Color COLOR_RED = Clay_Color{205, 66, 66, 255};
  const Clay_Color COLOR_GREEN = Clay_Color{66, 205, 66, 255};
  const Clay_Color COLOR_BLUE = Clay_Color{66, 66, 205, 255};
  const Clay_Color COLOR_CYAN = Clay_Color{0, 255, 255, 255};
  const Clay_Color COLOR_ORANGE = Clay_Color{225, 138, 50, 255};

  inline void spacer() {
    CLAY({
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
        },
    });
  }


  inline bool ButtonWasClicked(Clay_String id_str, u32 index) {
    auto id = Clay_GetElementIdWithIndex(id_str, index);
    bool buttonIsHovered = Clay_PointerOver(id);
    if (buttonIsHovered && IsMouseButtonPressed(0)) {
      return true;
    }
    return false;
  }

  inline bool ButtonWasClicked(Clay_String id_str) {
    auto id = Clay_GetElementId(id_str);
    bool buttonIsHovered = Clay_PointerOver(id);
    if (buttonIsHovered && IsMouseButtonPressed(0)) {
      return true;
    }
    return false;
  }
};// namespace UI
