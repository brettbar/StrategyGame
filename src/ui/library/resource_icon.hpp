#pragma once

#include "../../../data/resources.hpp"
#include "../common.h"
#include "texture_button.hpp"

namespace UI {
  inline void resource_icon(Resources::Type type, Clay_String cs, u32 i) {

    CLAY({
      .id = CLAY_SIDI(cs, i),
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

      texture_label(resource_icon_path(type), {32, 32});

      if (Clay_PointerOver(Clay_GetElementIdWithIndex(cs, i))) {
        CLAY({
          .layout = {.padding = {8, 8}},
          .backgroundColor = COLOR_BLACK,
          .floating =
            {
              .attachPoints =
                {
                  .element = CLAY_ATTACH_POINT_LEFT_TOP,
                  .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
                },
              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
        }) {
          text_label(cs, 12);
        }
      }
    }
  }
}// namespace UI
