#pragma once

#include "../../../../world/components/settlement_component.hpp"
#include "../../../common.h"
#include "../../../library/texture_button.hpp"

namespace UI {
  enum class Action_SettlementContext {
    None,
    SpawnColonist,
    SpawnArmy,
    BuildFarm,
  };

  inline Action_SettlementContext settlement_context(
    Settlement::Component *selected_settlement
  ) {

    CLAY(
      CLAY_ID( "SettlementContext" ),
      CLAY_RECTANGLE( {
        .color = { 0, 0, 0, 200 },
        .cornerRadius = { 5 },
      } ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_FIT( { 512 } ),
            .height = CLAY_SIZING_PERCENT( 0.5 ),
          },
        .padding = { 16, 16 },
        .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      } ),
      CLAY_BORDER_OUTSIDE_RADIUS( 2, COLOR_WHITE, 5 )
    ) {

      // printf( "Name: %s\n", selected_settlement.name.c_str() );

      Clay_String cs = (Clay_String) {
        .length = strlen( selected_settlement->name.c_str() ),
        .chars = selected_settlement->name.c_str(),
      };

      CLAY_TEXT(
        cs,
        CLAY_TEXT_CONFIG( {
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = 32,
        } )
      );

      for ( u32 i = 0; i < selected_settlement->buildings.size(); i++ ) {
        Clay_String sn = (Clay_String) {
          .length =
            strlen( selected_settlement->buildings[i].name_str.c_str() ),
          .chars = selected_settlement->buildings[i].name_str.c_str(),
        };


        CLAY( CLAY_LAYOUT( {
          .sizing = { .width = CLAY_SIZING_GROW() },
          .childGap = 4,
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_CENTER,
              .y = CLAY_ALIGN_Y_CENTER,
            },
        } ) ) {
          texture_label( hstr{ "slot.png" }, { 64, 64 } );

          texture_label( hstr{ "arrow.png" }, { 26, 26 } );

          texture_label(
            hstr{ ( selected_settlement->buildings[i].name_str + "_icon.png" )
                    .c_str() },
            { 64, 64 }
          );

          texture_label( hstr{ "arrow.png" }, { 26, 26 } );

          texture_label( hstr{ "slot.png" }, { 64, 64 } );

          // CLAY_TEXT(
          //   sn,
          //   CLAY_TEXT_CONFIG( {
          //     .textColor = COLOR_WHITE,
          //     .fontId = 0,
          //     .fontSize = 32,
          //   } )
          // );
        }
      }
    }

    return Action_SettlementContext::None;
  }
}// namespace UI
