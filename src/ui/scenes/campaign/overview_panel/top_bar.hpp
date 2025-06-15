
#pragma once

#include "../../../../shared/common.hpp"
#include "../../../../shared/utils.hpp"
#include "../../../library/texture_button.hpp"

#include "../../../common.h"

#include "../time_panel.hpp"

namespace UI {
  inline void settlements_count(u32);
  inline void overview_count(Clay_String, hstr, vec2f);

  inline void top_bar(u32 num_settlements, u32 num_armies) {
    CLAY({
      .id = CLAY_ID("TopBar"),
      .layout =
        {
          .sizing =
            {.width = CLAY_SIZING_GROW(),
             .height = CLAY_SIZING_FIT(21 * UI_SCALE)},
          .padding =
            {
              uint16_t(4 * UI_SCALE),
            },
          .childGap = uint16_t(12 * UI_SCALE),
          .childAlignment =
            {
              .y = CLAY_ALIGN_Y_CENTER,
            },
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
      .border = grey_border(),
    }) {
      settlements_count(num_settlements);
      overview_count(
        CLAY_STRING("TopBar::ArmiesCount"), hstr{"armies_count.png"}, {9, 12}
      );
      overview_count(
        CLAY_STRING("TopBar::PopulationCount"),
        hstr{"population_count.png"},
        {18, 12}
      );
      overview_count(
        CLAY_STRING("TopBar::FoodCount"), hstr{"food_count.png"}, {14, 8}
      );

      UI::spacer();

      UI::time_panel(Global::state.timeScale);
    }
  }

  inline void settlements_count(u32 num_settlements) {
    CLAY({
      .id = CLAY_ID("TopBar::SettlementsCount"),
      .layout =
        {
          .childGap = 2,
          .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    }) {

      cstr num_settlements_s = u32_to_cstr(num_settlements);

      Clay_String amount_cs = {
        .length = static_cast<int32_t>(strlen(num_settlements_s)),
        .chars = num_settlements_s,
      };

      texture_button(
        CLAY_STRING("TopBar::SettlementsCount::Icon"),
        hstr{"settlements_count.png"},
        {14, 13}
      );
      CLAY_TEXT(
        amount_cs,
        CLAY_TEXT_CONFIG({
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = uint16_t(16 * UI_SCALE),
        })
      );
    }
  }

  inline void overview_count(Clay_String id, hstr icon, vec2f dims) {
    CLAY({
      .layout =
        {.childGap = uint16_t(2 * UI_SCALE),
         .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},
         .layoutDirection = CLAY_LEFT_TO_RIGHT},
    }) {
      texture_button(id, icon, dims);

      CLAY_TEXT(
        CLAY_STRING("0"),
        CLAY_TEXT_CONFIG({
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = uint16_t(16 * UI_SCALE),
        })
      );
    }
  }

};// namespace UI
