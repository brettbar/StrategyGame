#pragma once

#include "../../../shared/common.hpp"

#include "../../immediate.hpp"

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
    auto root_g = f.Grid( root_r, 4, 3 );

    auto grid = f.Grid( root_g->Slots( 9, 10 ), 4, 4 );

    f.TextLabel( grid->Slot( 0 ), "Actor", GREEN );
    bool okay_pressed = f.TextButton( grid->Slot( 2 ), "Settlement", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );
      return Action_ActorContext::SpawnSettlement;
    }

    f.Draw();

    return Action_ActorContext::None;
  }

  inline void DrawSettlementContext( Settlement::Component *settlement ) {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f.Grid( root_r, 4, 3 );

    auto grid = f.Grid( root_g->Slots( 9, 10 ), 4, 4, Fade( BLACK, 0.5f ) );

    f.TextLabel( grid->Slot( 0 ), settlement->name, GREEN );
    f.TextLabel(
      grid->Slot( 1 ), Settlement::dev_names[settlement->development], GREEN
    );
    f.TextLabel(
      grid->Slot( 2 ),
      std::to_string( settlement->population.current ).c_str(),
      GREEN
    );


    f.Draw();
  }

  inline void DrawDebug() {
    auto f = Iron::Forge();

    rect root_r =
      rect{ 0, 0, (f32) GetScreenWidth() / 3, (f32) GetScreenHeight() / 3 };

    auto grid = f.Grid( root_r, 3, 3 );

    f.TextLabel( grid->Slot( 0 ), "Settlement", GREEN );
    bool okay_pressed = f.TextButton( grid->Slot( 2 ), "Okay", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );

      str foo = "h: " + std::to_string( Iron::Watcher()->context.hot ) +
                ", a: " + std::to_string( Iron::Watcher()->context.active ) +
                "\n";

      printf( "%s", foo.c_str() );
    }

    f.Draw();
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
