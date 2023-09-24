#pragma once

#include "../../../shared/common.hpp"

#include "../../builders.hpp"
#include "../../element.hpp"
#include "../../ui_utils.hpp"

namespace UI
{
  inline sptr<Element> CreateActorContextPanel()
  {
    return GridPanel( "actor_context_panel", 4, 3 )
      .StartsDisabled()
      .Background( Fade( BLACK, 0.5 ) )
      .On(
        InterfaceUpdate::ID::ActorContext,
        []( Element &self, InterfaceUpdate::Update update ) {
          printf( "InterfaceUpdate::ID::ActorContext %d\n", update.condition );

          if ( update.condition )
            self.Enable();
          else
            self.Disable();
        }
      )
      .Children( {
        GridSlot(
          { 0, 0, 0, 0 },
          GridPanel( "actor_actions_panel", 1, 1 )
            .Children( { GridSlot(
              { 0, 0, 0, 0 },
              TextButton( "actor_spawn_settlement_button", "Spawn?", 26 )
                .Clickable( false )
                .Background( RED )
                .SetEvent( InterfaceEvent::Data(
                  InterfaceEvent::ID::ActorSpawnSettlment
                ) )
                .On(
                  InterfaceUpdate::ID::ActorCanSpawnSettlement,
                  []( Element &self, InterfaceUpdate::Update update ) {
                    self.UpdateClickable( update.condition );

                    if ( update.condition )
                      self.UpdateBackground( GREEN );
                    else
                      self.UpdateBackground( RED );
                  }
                )
            ) } )
        ),
      } );
  }
}// namespace UI
