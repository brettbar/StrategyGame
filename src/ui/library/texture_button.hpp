#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../common.h"
#include "text_label.hpp"
#include <cstdint>
#include <raylib.h>


namespace UI {
  inline Clay_ImageElementConfig image(hstr texture_id, vec2f dimensions) {
    return {
      .imageData = (void *) &Global::texture_cache[texture_id]->texture,
    };
  }

  inline void tooltip(vec2f pos, Clay_String label) {
    CLAY({
      .id = CLAY_ID("Campaign::Tooltip"),
      .layout =
        {
          .padding = CLAY_PADDING_ALL(uint16_t(4 * UI_SCALE)),
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
      .floating =
        {
          .offset = {.x = pos.x, .y = pos.y},
          .zIndex = INT16_MAX,
          .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
          .attachTo = CLAY_ATTACH_TO_ROOT,
        },
      .border = border(COLOR_WHITE),
    }) {
      text_label(label, 12);
    }
  }

  inline bool texture_button(
    Clay_String cs,
    hstr texture_id,
    vec2f dimensions,
    u32 i = 0
  ) {
    auto hid = CLAY_SIDI_LOCAL(cs, i);
    CLAY({
      .id = hid,
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(dimensions.x * UI_SCALE),
              .height = CLAY_SIZING_FIXED(dimensions.y * UI_SCALE),
            },
        },
      .image = image(texture_id, dimensions),
    });

    return button_was_clicked(hid);
  }

  inline void texture_label(hstr texture_id, vec2f dimensions) {
    CLAY({
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(dimensions.x * UI_SCALE),
              .height = CLAY_SIZING_FIXED(dimensions.y * UI_SCALE),
            },
        },
      .image = image(texture_id, dimensions),
    });
  }

  inline bool texture_w_tooltip(
    Clay_String cs,
    Clay_String label,
    hstr texture_id,
    vec2f dimensions,
    u32 i = 0
  ) {
    auto hid = CLAY_SIDI_LOCAL(cs, i);

    CLAY({
      .id = hid,
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(dimensions.x * UI_SCALE),
              .height = CLAY_SIZING_FIXED(dimensions.y * UI_SCALE),
            },
        },
      .image = image(texture_id, dimensions),
    }) {
      // Clay_Vector2 mouse_pos = Clay_PointerData().position;
      vec2f mouse_pos = GetMousePosition();
      vec2f tooltip_coords = {
        .x = mouse_pos.x + (8 * UI_SCALE),
        .y = mouse_pos.y + (4 * UI_SCALE),
      };

      bool hovered = Clay_Hovered();

      if (hovered) {
        tooltip(tooltip_coords, label);
      }
    }

    return button_was_clicked(hid);
  }

  // CLAY({
  //   .layout =
  //     {
  //       .sizing =
  //         {
  //           .width = CLAY_SIZING_GROW(48 * UI_SCALE),
  //         },
  //       .padding = CLAY_PADDING_ALL(uint16_t(4 * UI_SCALE)),
  //       .childAlignment =
  //         {
  //           .x = CLAY_ALIGN_X_CENTER,
  //           .y = CLAY_ALIGN_Y_CENTER,
  //         },
  //     },
  //   .backgroundColor = COLOR_BLACK,
  //   .floating =
  //     {
  //       .attachPoints =
  //         {
  //           .element = CLAY_ATTACH_POINT_LEFT_TOP,
  //           .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
  //         },
  //       .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
  //       .attachTo = CLAY_ATTACH_TO_PARENT,
  //     },
  //   .border =
  //     {
  //       .color = COLOR_WHITE,
  //       .width = CLAY_BORDER_ALL(uint16_t(1 * UI_SCALE)),
  //     },
  // }) {
  //   text_label(label, 6);
  // }

}// namespace UI
