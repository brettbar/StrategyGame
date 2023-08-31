
#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"
#include <raylib.h>

namespace UI
{
  inline sptr<Element> CreateSinglePlayerLobby()
  {
    return {
      GridPanel( "singleplayer_lobby_root", 3, 3 )
        .FixedSize( GetScreenWidth(), GetScreenHeight() )
        .SetChildren( { GridPanelElement::Slot{
          { 1, 1, 1, 1 },
          GridPanel( "singleplayer_lobby", 1, 3 )
            .SetChildren( {
              GridPanelElement::Slot{
                { 0, 0, 0, 0 },
                TextButton(
                  "singleplayer_faction_label", "Select your faction", 32
                )
                  .Background( GREEN )
                  .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage ),
              },
              GridPanelElement::Slot{
                { 0, 0, 1, 1 },
                TextButton(
                  "player_0_faction_selected", "Waiting to Select Faction", 32
                )
                  .Background( GRAY )
                  .On(
                    InterfaceUpdate::ID::PlayerSelectedFaction,
                    []( Element &self, InterfaceUpdate::Update update ) {
                      if ( self.id == update.player_id + "_faction_selected" )
                      {
                        printf( "PlayerSelectedFaction !!!!!!!!!!!\n" );
                        printf( "update_txt: %s\n", update.update_txt.c_str() );
                        self.UpdateText( update.update_txt );
                        self.UpdateBackground(
                          GetPrimaryFactionColor( update.update_txt )
                        );
                      }
                    }
                  ),
              },

              GridPanelElement::Slot{
                { 0, 0, 2, 2 },
                TextButton(
                  "singleplayer_lobby_started_game", "Start Game", 32
                )
                  .Background( BLUE )
                  .SetEvent( InterfaceEvent::ID::SinglePlayerLobbyStartGame ),
              },
            }

            ),
        } } ) };
  }
};// namespace UI
