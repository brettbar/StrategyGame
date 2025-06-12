#pragma once

#include "../../../../world/components/settlement_component.hpp"
#include "../../../common.h"
#include "../../../library/resource_icon.hpp"
#include "../../../library/texture_button.hpp"

namespace UI {
  enum class Action_SettlementContext {
    None,
    SpawnColonist,
    SpawnArmy,
  };

  inline void buildings_list(list<Settlement::Building>);

  inline Action_SettlementContext settlement_context(
    Settlement::Component *selected_settlement
  ) {

    CLAY({
      .id = CLAY_ID("SettlementContext"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .padding = {16, 16},
          .childGap = 4,
          .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP},
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
    }) {
      Clay_String cs = Clay_String{
        .length =
          static_cast<int32_t>(strlen(selected_settlement->name.c_str())),
        .chars = selected_settlement->name.c_str(),
      };

      CLAY_TEXT(
        cs,
        CLAY_TEXT_CONFIG({
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = 32,
        })
      );

      buildings_list(selected_settlement->buildings);
    }

    return Action_SettlementContext::None;
  }

  inline void buildings_list(list<Settlement::Building> buildings) {
    for (u32 i = 0; i < buildings.size(); i++) {
      CLAY({
        .layout =
          {
            .sizing = {.width = CLAY_SIZING_GROW()},
            .childGap = 4,
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      }) {
        texture_label(hstr{"slot.png"}, {32, 32});

        texture_label(hstr{"arrow.png"}, {13, 13});

        // texture_label(
        //   Buildings::building_icon_path(buildings[i].type), {32, 32}
        // );
        const char *label = Buildings::building_name_str(buildings[i].type);

        Clay_String cs = Clay_String{
          .length = static_cast<int32_t>(strlen(label)),
          .chars = label,
        };

        texture_w_tooltip(
          CLAY_STRING("BuildingIcon"),
          cs,
          Buildings::building_icon_path(buildings[i].type),
          {32, 32},
          i
        );

        texture_label(hstr{"arrow.png"}, {13, 13});

        auto recipes = Buildings::recipes_for_building(buildings[i].type);

        auto current_recipe = buildings[i].current_recipe;

        for (const auto &output: current_recipe.outputs) {
          const char *resource = Resources::ResourceStr(output.resource);

          Clay_String cs = Clay_String{
            .length = static_cast<int32_t>(strlen(resource)),
            .chars = resource,
          };

          resource_icon(output.resource, cs, i);
        }
      }
    }
  }
}// namespace UI
