#pragma once

#include "../../../library/texture_button.hpp"
#include "construction/construction_tab.hpp"
#include "resources_tab.hpp"


namespace UI {
  enum class Action_Overview_t {
    None,
    CloseTab,
    Construction,
  };
  struct Action_Overview {
    Action_Overview_t type;
    Buildings::Building building;
  };

  inline Action_Overview overview_content(Clay_String current_tab) {
    opt<Buildings::Building> building_to_build = std::nullopt;

    Action_Overview action = Action_Overview{
      .type = Action_Overview_t::None,
    };

    // @todo refactor this shit
    // coming from overview_tabs overall height
    f32 max_height = ((1 * 8) + (32 * 8) + 64 - (21 + 1)) * UI_SCALE;

    CLAY({
      .id = CLAY_ID("OverviewPanel::Content"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(243 * UI_SCALE),
              .height = CLAY_SIZING_FIXED(max_height),
            },
          .padding = CLAY_PADDING_ALL(uint16_t(8 * UI_SCALE)),
          .childGap = uint16_t(8 * UI_SCALE),
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
      .border = border(COLOR_GREY),
    }) {
      CLAY({
        .id = CLAY_ID("OverviewPanel::TopBar"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_FIT(15, 15 * UI_SCALE),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      }) {
        CLAY_TEXT(
          current_tab,
          CLAY_TEXT_CONFIG({
            .textColor = COLOR_WHITE,
            .fontSize = uint16_t(12 * UI_SCALE),
          })
        );

        spacer();

        if (texture_button(CLAY_STRING("Exit"), hstr{"exit.png"}, {15, 15})) {
          action = Action_Overview{
            .type = Action_Overview_t::CloseTab,
          };
        }
      }

      if (std::string(current_tab.chars) == "Construction") {
        building_to_build = UI::construction_tab();

        if (building_to_build.has_value()) {
          action = Action_Overview{
            .type = Action_Overview_t::Construction,
            .building = building_to_build.value(),
          };
        }
      } else if (std::string(current_tab.chars) == "Resources") {
        UI::resources_tab();
      }
    }

    return action;
  }
}// namespace UI
