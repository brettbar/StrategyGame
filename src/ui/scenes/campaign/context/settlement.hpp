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
    Settlement::Component selected_settlement
  ) {

    CLAY(
      CLAY_ID( "SettlementContext" ),
      CLAY_RECTANGLE( {
        .color = COLOR_SLATE,
        .cornerRadius = { 5 },
      } )
    ) {

      printf( "Name: %s\n", selected_settlement.name.c_str() );
      // const char *name = selected_settlement.name.c_str();
      //
      // Clay_String cs = Clay_String{
      //   .length = strlen( name ),
      //   .chars = name,
      // };
      //
      // CLAY_TEXT( cs, {} );

      for ( u32 i = 0; i < selected_settlement.buildings.size(); i++ ) {
        auto building = selected_settlement.buildings[i];
        printf( "building %s\n", building.name );
      }
    }

    return Action_SettlementContext::None;
  }
}// namespace UI
