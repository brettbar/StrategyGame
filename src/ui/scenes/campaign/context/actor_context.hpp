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
    CLAY( CLAY_LAYOUT( { .layoutDirection = CLAY_LEFT_TO_RIGHT } ) ) {
      switch ( actor->data.type ) {
        case Actor::Type::Colonist: {
          RenderMenuButton(
            CLAY_STRING( "Colonist::ClaimProvince" ), CLAY_STRING( "Claim" ), 0
          );

          RenderMenuButton(
            CLAY_STRING( "Colonist::SpawnSettlement" ),
            CLAY_STRING( "Settlement" ),
            0
          );
        } break;
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
