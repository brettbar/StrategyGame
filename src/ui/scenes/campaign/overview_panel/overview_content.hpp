#pragma once

#include "../../../library/texture_button.hpp"
#include "construction/construction_tab.hpp"
#include "resources_tab.hpp"


namespace UI {
  enum class OverviewAction_t {
    None,
    CloseTab,
    Construction,
  };
  struct OverviewAction {
    OverviewAction_t type;
    Buildings::Building building;
  };

  inline OverviewAction overview_content(Clay_String current_tab) {
    opt<Buildings::Building> building_to_build = std::nullopt;

    // @todo refactor this shit
    // coming from overview_tabs overall height
    f32 max_height = ((32 * 8) + 64 - 21) * UI_SCALE;


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
      .border = grey_border(),
    }) {
      CLAY({
        .id = CLAY_ID("OverviewPanel::Content::Banner"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_FIT(24 * UI_SCALE),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_RIGHT,
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

        texture_button(CLAY_STRING("Exit"), hstr{"exit.png"}, {15, 15});
      }

      if (std::string(current_tab.chars) == "Construction") {
        building_to_build = UI::construction_tab();
      } else if (std::string(current_tab.chars) == "Resources") {
        UI::resources_tab();
      }
    }

    if (ButtonWasClicked(CLAY_STRING("Exit"))) {
      return OverviewAction{
        .type = OverviewAction_t::CloseTab,
      };
    }


    if (building_to_build.has_value()) {
      return OverviewAction{
        .type = OverviewAction_t::Construction,
        .building = building_to_build.value(),
      };
    }

    return {.type = OverviewAction_t::None};
  }
}// namespace UI
