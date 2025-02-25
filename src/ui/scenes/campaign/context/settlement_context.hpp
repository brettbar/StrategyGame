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

  inline Action_SettlementContext settlement_context(
    Settlement::Component *selected_settlement
  ) {

    CLAY( {
      .id = CLAY_ID( "SettlementContext" ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( 768 ),
              // .height = CLAY_SIZING_PERCENT( 0.5 ),
              .height = CLAY_SIZING_FIT( 256 ),
            },
          .padding = { 16, 16 },
          .childGap = 4,
          .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = { 0, 0, 0, 200 },
      .cornerRadius = { 5, 5, 5, 5 },
      .border =
        {
          .color = COLOR_WHITE,
          .width = { 2, 2, 2, 2 },
        },
    } ) {

      // printf( "Name: %s\n", selected_settlement.name.c_str() );

      Clay_String cs = Clay_String{
        .length =
          static_cast<int32_t>( strlen( selected_settlement->name.c_str() ) ),
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


        Clay_String sn = Clay_String{
          .length = static_cast<int32_t>( strlen(
            Buildings::building_name_str( selected_settlement->buildings[i].type
            )
              .c_str()
          ) ),

          .chars = ( Buildings::building_name_str(
                       selected_settlement->buildings[i].type
          )
                       .c_str() ),
        };


        CLAY( {
          .layout =
            {
              .sizing = { .width = CLAY_SIZING_GROW() },
              .childGap = 4,
              .childAlignment =
                {
                  .x = CLAY_ALIGN_X_CENTER,
                  .y = CLAY_ALIGN_Y_CENTER,
                },
            },
        } ) {
          texture_label( hstr{ "slot.png" }, { 64, 64 } );

          texture_label( hstr{ "arrow.png" }, { 26, 26 } );

          texture_label(
            Buildings::building_icon_path(
              selected_settlement->buildings[i].type
            ),
            { 64, 64 }
          );

          texture_label( hstr{ "arrow.png" }, { 26, 26 } );

          auto recipes = Buildings::recipes_for_building(
            selected_settlement->buildings[i].type
          );

          auto current_recipe =
            recipes[selected_settlement->buildings[i].current_recipe];

          for ( const auto &output: current_recipe.outputs ) {
            resource_icon( output.resource, { 64, 64 }, i );
          }
        }
      }
    }

    return Action_SettlementContext::None;
  }
}// namespace UI
