#pragma once

#include "../../../../world/components/settlement_component.hpp"
#include "../../../common.h"

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
          .textColor = { 255, 255, 255, 255 },
          .fontId = 0,
          .fontSize = 32,
        } )
      );

      for ( u32 i = 0; i < selected_settlement->buildings.size(); i++ ) {
        auto building = selected_settlement->buildings[i];
        printf( "building %s\n", building.name );
      }
    }

    return Action_SettlementContext::None;
  }
}// namespace UI
