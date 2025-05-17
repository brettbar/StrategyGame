
#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../shared/common.hpp"
#include "../../../../shared/utils.hpp"
#include "../../../library/texture_button.hpp"

#include "../../../common.h"

namespace UI {
  inline void top_bar( u32 num_settlements, u32 num_armies ) {
    CLAY( {
      .id = CLAY_ID( "TopBar" ),
      .layout =
        {
          .sizing =
            { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT( 42 ) },
          .padding = { 4, 4, 4, 4 },
          .childGap = 4,
          .childAlignment =
            {
              .y = CLAY_ALIGN_Y_CENTER,
            },
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
    } ) {
      CLAY( {
        .id = CLAY_ID( "TopBar::SettlementsCount" ),
        .layout =
          {
            .childGap = 2,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
      } ) {

        cstr num_settlements_s = u32_to_cstr( num_settlements );

        Clay_String amount_cs = {
          .length = static_cast<int32_t>( strlen( num_settlements_s ) ),
          .chars = num_settlements_s,
        };

        texture_button(
          CLAY_STRING( "TopBar::SettlementsCount::Icon" ),
          hstr{ "settlements_count.png" },
          { 14, 13 }
        );
        CLAY_TEXT(
          amount_cs,
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
            .fontId = 0,
            .fontSize = 32,
          } )
        );
      }

      CLAY( {
        .id = CLAY_ID( "TopBar::ArmiesCount" ),
        .layout =
          { .childGap = 2,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT },
      } ) {
        texture_button(
          CLAY_STRING( "TopBar::ArmiesCount::Icon" ),
          hstr{ "armies_count.png" },
          { 9, 12 }
        );
        CLAY_TEXT(
          CLAY_STRING( "0" ),
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
            .fontId = 0,
            .fontSize = 32,
          } )
        );
      }
    }
  }

};// namespace UI
