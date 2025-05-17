#pragma once

#include "../../../../world/components/actor_component.hpp"
#include "../../../common.h"
#include "../../../library/text_button.hpp"

namespace UI {
  enum class Action_ActorContext {
    None,
    ClaimProvince,
    SpawnSettlement,
  };

  inline Action_ActorContext actor_context( Actor::Component *actor ) {

    CLAY( {
      .id = CLAY_ID( "ActorContext" ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( 512 ),
              .height = CLAY_SIZING_PERCENT( 0.3 ),
            },
          .padding = { 16, 16 },
          .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = { 0, 0, 0, 200 },
      // .cornerRadius = { 5 },
    } ) {

      Clay_String cs = Clay_String{
        .length = static_cast<int32_t>( strlen( actor->data.name.c_str() ) ),
        .chars = actor->data.name.c_str(),
      };

      CLAY_TEXT(
        cs,
        CLAY_TEXT_CONFIG( {
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = 32,
        } )
      );

      CLAY( { .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } } ) {
        switch ( actor->data.type ) {
          case Actor::Type::Colonist: {
            text_button_small(
              CLAY_STRING( "Colonist::ClaimProvince" ),
              CLAY_STRING( "Claim" ),
              0
            );

            text_button_small(
              CLAY_STRING( "Colonist::SpawnSettlement" ),
              CLAY_STRING( "Settlement" ),
              0
            );
          } break;
        }
      }
    }


    switch ( actor->data.type ) {
      case Actor::Type::Colonist: {
        if ( ButtonWasClicked( CLAY_STRING( "Colonist::ClaimProvince" ) ) ) {
          return Action_ActorContext::ClaimProvince;
        }

        if ( ButtonWasClicked( CLAY_STRING( "Colonist::SpawnSettlement" ) ) ) {
          return Action_ActorContext::SpawnSettlement;
        }
      } break;
    }


    return Action_ActorContext::None;
  }
};// namespace UI
