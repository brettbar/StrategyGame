#pragma once

#include "../../../data/resources.hpp"
#include "../common.h"
#include "texture_button.hpp"

namespace UI {
inline void resource_icon(Resources::Type type, Clay_String cs, u32 i) {
  CLAY({
    .layout =
      {
        .childAlignment =
          {
            .x = CLAY_ALIGN_X_CENTER,
            .y = CLAY_ALIGN_Y_CENTER,
          },
      },
    .image = image(hstr{"slot.png"}, {32, 32}),
  }) {

    texture_w_tooltip(
      CLAY_STRING("ResourceIcon"),
      cs,
      Resources::resource_icon_path(type),
      {32, 32}
    );
  }
}

inline void resource_icon_w_quantity(
  Resources::Type type,
  u32 quantity,
  Clay_String cs,
  u32 i
) {
  CLAY({
    .layout =
      {
        .childAlignment =
          {
            .x = CLAY_ALIGN_X_CENTER,
            .y = CLAY_ALIGN_Y_CENTER,
          },
      },
    .image = image(hstr{"slot.png"}, {32, 32}),
  }) {

    texture_w_tooltip(
      CLAY_STRING("ResourceIcon"),
      cs,
      Resources::resource_icon_path(type),
      {32, 32},
      i
    );

    CLAY({
      .floating = {
        .offset = {.x = (20 * UI_SCALE), .y = (20 * UI_SCALE)},
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
    }) {

      cstr quant = u32_to_cstr(quantity);
      auto quant_cs = cstr_to_cs(quant);
      text_label(quant_cs, 12);
    }
  }
}
}// namespace UI
