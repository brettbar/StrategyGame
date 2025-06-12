#pragma once

#include "../../../../../data/resources.hpp"
#include "../../../../shared/common.hpp"
#include "../../../common.h"
#include "../../../library/text_label.hpp"
#include "../../../library/texture_button.hpp"

#include "../../../../world/systems/player_system.hpp"

namespace UI {


  inline void resource_quantity(Clay_String amount_cs, vec2f dimensions) {
    CLAY({
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(dimensions.x * UI_SCALE),
              .height = CLAY_SIZING_FIXED(dimensions.y * UI_SCALE),
            },
          .childAlignment =
            {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
        },
    }) {
      text_label(amount_cs, 12);
    }
  }

  inline void resource_rows(Resources::Type resources_count) {
    vec2f dimensions = {32, 32};

    entt::entity local_player = Player::System::GetEntityOfPlayer("player_0");

    auto resource_map =
      Resource::System::get_resources_for_player(local_player);

    for (u32 i = 0; i < (u32) resources_count; i++) {
      Resources::Type resource_t = (Resources::Type) i;

      u32 amount_stored = resource_map[resource_t];

      cstr amount_stored_s = u32_to_cstr(amount_stored);

      Clay_String amount_cs = {
        .length = static_cast<int32_t>(strlen(amount_stored_s)),
        .chars = amount_stored_s,
      };

      Clay_String temp_else = {
        .length = static_cast<int32_t>(strlen("0")),
        .chars = "0",
      };


      CLAY({
        .layout =
          {
            .childGap = 4,
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
      }) {
        // 1. Icon
        texture_label(resource_icon_path(resource_t), {32, 32});

        // 2. Stored
        resource_quantity(amount_cs, dimensions);
        // 3.  Net Change
        resource_quantity(temp_else, dimensions);
        // 4. Usage
        resource_quantity(temp_else, dimensions);
        // 5. Imports
        resource_quantity(temp_else, dimensions);
      }
    }
  }

  inline void resources_headers() {
    list<hstr> resource_headers = {
      "resources_header_icons.png",
      "resources_header_stockpile.png",
      "resources_header_net_change.png",
      "resources_header_usage.png",
      "resources_header_imports.png",
    };

    list<vec2f> dimensions = {
      {9, 12},
      {17, 7},
      {16, 7},
      {13, 7},
      {29, 7},
    };

    CLAY({
      .id = CLAY_ID("ResourceHeaders"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              // .height = CLAY_SIZING_FIXED( 18 * UI_SCALE ),
            },
          .childGap = 4,
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    }) {
      for (u32 i = 0; i < resource_headers.size(); i++) {
        CLAY({
          .layout =
            {
              .sizing =
                {
                  .width = CLAY_SIZING_FIXED(32 * UI_SCALE),
                  .height = CLAY_SIZING_FIXED(18 * UI_SCALE),
                },
              .childAlignment =
                {
                  .x = CLAY_ALIGN_X_CENTER,
                  .y = CLAY_ALIGN_Y_CENTER,
                },
            },
        }) {
          texture_label(resource_headers[i], dimensions[i]);
        }
      }
    }
  }

  inline void resources_tab() {
    CLAY({
      .id = CLAY_ID("ResourcesTab"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .childGap = 4,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    }) {
      resources_headers();

      CLAY({
        .id = CLAY_ID("ResourcesRows"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
              },
            .childGap = 4,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
      }) {
        resource_rows(Resources::Type::COUNT);
      }
    }
  }
}// namespace UI
