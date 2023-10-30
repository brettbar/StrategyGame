#pragma once

#include "../../../shared/common.hpp"

#include "../../builders.hpp"
#include "../../ui_utils.hpp"

#include "actor_context.hpp"
#include "settlement_context/settlement_context.hpp"
#include <raylib.h>


namespace UI {

  inline void DrawActorContext( Actor::Component *actor ) {
    auto f = Iron::Forge();

    rect r = rect{ 0, 0, 250, 250 };
    auto grid = f->Grid( r, 3, 3 );

    f->TextLabel( grid->Slot( 0 ), "Actor", GREEN );
    bool okay_pressed = f->TextButton( grid->Slot( 2 ), "Okay", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );
    }

    f->Draw();
  }

  inline void DrawSettlementContext( Settlement::Component *settlement ) {

    auto f = Iron::Forge();

    rect r = rect{ 0, 0, 250, 250 };
    auto grid = f->Grid( r, 3, 3 );


    f->TextLabel( grid->Slots( 0, 4 ), "Nevermind", RED );

    bool okay_pressed = f->TextButton( grid->Slot( 2 ), "Okay", BLUE );


    if ( okay_pressed ) {
      printf( "okay pressed\n" );
    }


    {
      f->Draw();


      // if ( settlement ) {
      //   Iron::DrawTextButton( rectangle, settlement->name, BLUE );
      // } else {
      //   Iron::DrawTextButton( rectangle, "Uninhabited", BLUE );
      // }
    }
  }

  inline sptr<Element> CreateCampaignUI() {
    return {
      GridPanel( "campaign_root", 4, 3 )
        .FixedSize( GetScreenWidth(), GetScreenHeight() )
        .Children( {
          // GridPanelElement::Slot{
          //   { 1, 2, 2, 2 },
          //   CreateSettlementContextPanel(),
          // },
          GridPanelElement::Slot{
            { 1, 2, 2, 2 },
            CreateActorContextPanel(),
          },
        } ),
    };
  }

};// namespace UI
