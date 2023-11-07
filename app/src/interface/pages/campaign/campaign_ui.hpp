#pragma once

#include "../../../shared/common.hpp"

#include "../../irongui/state.hpp"

#include "actor_context.hpp"
#include "settlement_context/settlement_context.hpp"
#include <raylib.h>


namespace UI {

  enum class Action_ActorContext {
    None,
    SpawnSettlement,
  };

  inline Action_ActorContext DrawActorContext( Actor::Component *actor ) {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 3 );

    auto grid = f->Grid( root_g->Slots( 9, 10 ), 4, 4 );

    f->TextLabel( grid->Slot( 0 ), "Actor", GREEN );
    bool okay_pressed = f->TextButton( grid->Slot( 2 ), "Settlement", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );
      return Action_ActorContext::SpawnSettlement;
    }

    return Action_ActorContext::None;
  }


  // inline sptr<Element> CreateCampaignUI() {
  //   return {
  //     GridPanel( "campaign_root", 4, 3 )
  //       .FixedSize( GetScreenWidth(), GetScreenHeight() )
  //       .Children( {
  //         // GridPanelElement::Slot{
  //         //   { 1, 2, 2, 2 },
  //         //   CreateSettlementContextPanel(),
  //         // },
  //         GridPanelElement::Slot{
  //           { 1, 2, 2, 2 },
  //           CreateActorContextPanel(),
  //         },
  //       } ),
  //   };
  // }

};// namespace UI
