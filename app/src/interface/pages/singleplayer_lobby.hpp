
#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI
{
  inline std::vector<Element> CreateSinglePlayerLobby()
  {
    return {
      Panel( "singleplayer_lobby" )
        .SetAnchor( Anchor::Centered )
        .SetAxis( Axis::Column )
        .Children( {
          TextButton( "singleplayer_faction_label" )
            .SetText( "Select your faction", 32 )
            .Background( GREEN )
            .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage ),
          TextButton( "singleplayer_faction_selected" )
            .SetText( "Waiting to Select Faction", 32 )
            .Background( GRAY )
            .On(
              InterfaceUpdate::ID::PlayerSelectedFaction,
              []( Element &self, InterfaceUpdate::Update update ) {
                if ( self.id == update.player_id + "_select_faction" )
                {
                  self.UpdateText( update.updated_txt );
                  self.UpdateBackground(
                    GetPrimaryFactionColor( update.updated_txt )
                  );
                }
              }
            ),
          TextButton( "singleplayer_lobby_started_game" )
            .SetText( "Start Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::SinglePlayerLobbyStartGame ),
        }

        ),
    };
  }
};// namespace UI
