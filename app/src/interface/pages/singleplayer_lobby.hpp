
#pragma once

#include "../../shared/common.hpp"

#include "../ui_builder.hpp"

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
            .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage )
            .build(),
          TextButton( "singleplayer_faction_selected" )
            .SetText( "Waiting to Select Faction", 32 )
            .Background( GRAY )
            .ListensFor( { InterfaceUpdate::ID::FactionSelected } )
            .build(),
          TextButton( "singleplayer_lobby_started_game" )
            .SetText( "Start Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::SinglePlayerLobbyStartGame )
            .build(),
        }

        )
        .build(),
    };
  }
};// namespace UI
