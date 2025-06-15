#pragma once

#include "../shared/common.hpp"
#include "clay/clay.h"
#include <raylib.h>

namespace UI {
  inline float UI_SCALE = 2.0f;

  // inline void set_ui_scale(f32 height_dpi) {
  //   // f32 base_dpi = 96.0f;
  //   f32 base_dpi = 48.0f;
  //
  //   UI_SCALE = height_dpi / base_dpi;
  //
  //   if (UI_SCALE < 2.0f)
  //     UI_SCALE = 2.0f;
  // }

  inline void set_ui_scale_by_screen_height(f32 screen_height) {
    if (screen_height > 1440) {
      UI_SCALE = 4.0f;
    } else if (screen_height > 1080) {
      UI_SCALE = 2.0f;
    } else {
      UI_SCALE = 1.0f;
    }
  }


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
  const Clay_Color COLOR_GREY = Clay_Color{137, 137, 137, 255};

  inline Clay_BorderElementConfig grey_border() {
    uint16_t border_w = 1.0f * UI_SCALE;

    return {
      .color = COLOR_GREY,
      .width =
        {
          .left = border_w,
          .right = border_w,
          .top = border_w,
          .bottom = border_w,
        },
    };
  }

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

  inline bool ButtonWasClicked(Clay_ElementId id) {
    bool buttonIsHovered = Clay_PointerOver(id);
    if (buttonIsHovered && IsMouseButtonPressed(0)) {
      return true;
    }
    return false;
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

  // @todo verify this works
  // it does in the settlement context at least :shrug:
  inline Clay_String cstr_to_cs(const char *input) {
    return Clay_String{
      .length = static_cast<int32_t>(strlen(input)),
      .chars = input,
    };
  }
};// namespace UI
