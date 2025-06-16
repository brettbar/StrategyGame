#pragma once

#include "../../shared/common.hpp"

#include "../common.h"
#include <cstdint>

namespace UI {

  inline bool text_button_lrg(
    Clay_String id_str,
    Clay_String text,
    u32 i = 0,
    Clay_Color color = {140, 140, 140, 255}
  ) {
    CLAY({
      .id = (Clay__HashString(id_str, i, 0)),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT(128 * UI_SCALE),
              .height = CLAY_SIZING_FIT(24 * UI_SCALE),
            },
          .padding = {16, 16},
          .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
        },
      .backgroundColor = color,
      .cornerRadius = {5, 5, 5, 5},
    }) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG({
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = uint16_t(16.0 * UI_SCALE),
        })
      );
    }

    return button_was_clicked(id_str, i);
  }

  inline bool text_button_small(
    Clay_String id_str,
    Clay_String text,
    u32 i = 0,
    Clay_Color color = {140, 140, 140, 255}
  ) {
    CLAY({
      .id = (Clay__HashString(id_str, i, 0)),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT(64 * UI_SCALE),
              .height = CLAY_SIZING_FIT(18 * UI_SCALE),
            },
          .padding = {16, 16},
          .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
        },
      .backgroundColor = color,
      .cornerRadius = {5, 5, 5, 5},
    }) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG({
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = uint16_t(8 * UI_SCALE),
        })
      );
    }

    return button_was_clicked(id_str, i);
  }


}// namespace UI
