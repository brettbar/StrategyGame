#pragma once

#include "../../../shared/common.hpp"

#include "../../ui_utils.hpp"

#include "../../../world/actor/component.hpp"

#include "../../irongui/forge.hpp"

namespace UI {
  enum class Action_ActorContext {
    None,
    ClaimProvince,
    SpawnSettlement,
  };

  inline Action_ActorContext ActorContext( Actor::Component *actor ) {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 3 );

    auto grid = f->Grid( root_g->Slots( 9, 10 ), 4, 4, Fade( BLACK, 0.5 ) );


    switch ( actor->data.type ) {
      case Actor::Type::Colonist: {
        f->TextLabel( grid->Slot( 0 ), "Colonist", GREEN );
        bool claim_province = f->TextButton( grid->Slot( 1 ), "Claim", RED );
        bool spawn_settlement =
          f->TextButton( grid->Slot( 2 ), "Settlement", BLUE );

        if ( claim_province ) {
          return Action_ActorContext::ClaimProvince;
        }

        if ( spawn_settlement ) {
          return Action_ActorContext::SpawnSettlement;
        }
      } break;
      case Actor::Type::ArmyTierI: {
        f->TextLabel( grid->Slot( 0 ), "Army", GREEN );
        
      } break;
    }

    return Action_ActorContext::None;
  }


}// namespace UI
