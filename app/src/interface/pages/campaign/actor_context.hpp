#pragma once

#include "../../../shared/common.hpp"

#include "../../element.hpp"
#include "../../ui_utils.hpp"

namespace UI
{
  inline Element CreateActorContextPanel()
  {
    return Panel( "actor_context_panel" )
      .SetAnchor( Anchor::BottomMid )
      .Background( Fade( BLACK, 0.5 ) )
      .FixedSize( 800, 350 )
      .On(
        InterfaceUpdate::ID::ActorContext,
        []( Element &self, InterfaceUpdate::Update update ) {
          if ( update.condition )
            self.Enable();
          else
            self.Disable();
        }
      )
      .Children( {
        Panel( "actor_actions_panel" )
          .Children( {
            TextButton( "actor_spawn_settlement_button" )
              .Clickable( false )
              .Background( GREEN )
              .SetText( "Spawn?", 26 )
              .SetEvent(
                InterfaceEvent::Data( InterfaceEvent::ID::ActorSpawnSettlment )
              )
              .On(
                InterfaceUpdate::ID::ActorCanSpawnSettlement,
                []( Element &self, InterfaceUpdate::Update update ) {
                  self.UpdateClickable( update.condition );
                  if ( update.condition )
                    self.UpdateBackground( GREEN );
                  else
                    self.UpdateBackground( RED );
                }
              ),
          } ),
      } );
  }
}// namespace UI
