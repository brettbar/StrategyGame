#pragma once

#include "../../../../world/components/settlement_component.hpp"
#include "../../../common.h"
#include "../../../library/texture_button.hpp"
#include "../overview_panel/resources_tab.hpp"

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
      // .cornerRadius = { 5, 5, 5, 5 },
    }) {

      // printf( "Name: %s\n", selected_settlement.name.c_str() );

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
      Clay_String sn = Clay_String{
        .length = static_cast<int32_t>(
          strlen(Buildings::building_name_str(buildings[i].type).c_str())
        ),

        .chars = (Buildings::building_name_str(buildings[i].type).c_str()),
      };


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

        texture_label(
          Buildings::building_icon_path(buildings[i].type), {32, 32}
        );

        texture_label(hstr{"arrow.png"}, {13, 13});

        auto recipes = Buildings::recipes_for_building(buildings[i].type);

        auto current_recipe = recipes[buildings[i].current_recipe];

        for (const auto &output: current_recipe.outputs) {
          resource_icon(output.resource, i);
        }
      }
    }
  }
}// namespace UI
