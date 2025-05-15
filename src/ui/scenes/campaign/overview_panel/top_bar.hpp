
#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"

#include "../../../common.h"

namespace UI {
  inline void top_bar() {
    CLAY({
      .id = CLAY_ID( "TopBar" ),
      .layout =
        {
          .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT( 42) },
          .childGap = 4,
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
        .backgroundColor = COLOR_BLACK,
    }){
      CLAY({
        .id = CLAY_ID("TopBar::SettlementsCount"),
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        },
      }) {
        texture_button(CLAY_STRING("TopBar::SettlementsCount::Icon"), hstr{"settlements_count.png"}, {28, 26});
        CLAY_TEXT(
          CLAY_STRING("0"),
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
            .fontId = 0,
            .fontSize = 32,
          } )
        );
      }

      CLAY({
        .id = CLAY_ID("TopBar::ArmiesCount"),
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        },
      }) {
        texture_button(CLAY_STRING("TopBar::ArmiesCount::Icon"), hstr{"armies_count.png"}, {18, 24});
        CLAY_TEXT(
          CLAY_STRING("0"),
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
            .fontId = 0,
            .fontSize = 32,
          } )
        );
      }

    }

  }

};