#pragma once

#include "../../../shared/common.hpp"

#include "../../builders.hpp"
#include "../../element.hpp"
#include "../../ui_utils.hpp"

#include "../../../world/components/actor.hpp"

#include "../../irongui/state.hpp"

namespace UI {
  enum class Action_ActorContext {
    None,
    SpawnSettlement,
  };

  inline Action_ActorContext ActorContext( Actor::Component *actor ) {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 3 );

    auto grid = f->Grid( root_g->Slots( 9, 10 ), 4, 4, Fade( BLACK, 0.5 ) );

    f->TextLabel( grid->Slot( 0 ), "Actor", GREEN );
    bool okay_pressed = f->TextButton( grid->Slot( 2 ), "Settlement", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );
      return Action_ActorContext::SpawnSettlement;
    }

    return Action_ActorContext::None;
  }


}// namespace UI
