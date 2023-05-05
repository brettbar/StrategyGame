#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI
{
  inline std::vector<Element> CreateSlots( u32 start, u32 end )
  {
    std::vector<Element> slots = {};

    for ( u32 i = start; i < end; i++ )
    {
      std::string player_id = "player_" + std::to_string( i );

      Element panel =
        Panel( player_id + "_slot" )
          .SetAxis( Axis::Column )
          .Margins( { 16, 16, 0, 0 } )
          .Children( {
            TextButton( player_id + "_select_faction_btn" )
              .SetText( "Select Faction", 24 )
              .Background( GRAY )
              .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage )
              .Clickable( false )
              .On(
                InterfaceUpdate::ID::HostLobby,
                []( Element &self, InterfaceUpdate::Update update ) {
                  if ( self.id == update.player_id + "_select_faction_btn" )
                  {
                    self.UpdateClickable( true );
                    self.UpdateBackground( GREEN );
                  }
                }
              )
              .On(
                InterfaceUpdate::ID::JoinLobby,
                []( Element &self, InterfaceUpdate::Update update ) {
                  if ( self.id == update.player_id + "_select_faction_btn" )
                  {
                    self.UpdateClickable( true );
                    self.UpdateBackground( GREEN );
                  }
                  else
                  {
                    self.UpdateClickable( false );
                  }
                }
              )
              .On(
                InterfaceUpdate::ID::PlayerJoinedLobby,
                []( Element &self, InterfaceUpdate::Update update ) {
                  printf( "PlayerJoinedLobby!!!!!!!!\n" );
                  if ( self.id == update.player_id + "_select_faction_btn" )
                  {
                    self.UpdateBackground( PURPLE );
                  }
                }
              ),
            TextLabel( player_id + "_selected_faction_lbl" )
              .SetText( "Selecting Faction...", 24 )
              .Background( GRAY )
              .On(
                InterfaceUpdate::ID::PlayerSelectedFaction,
                []( Element &self, InterfaceUpdate::Update update ) {
                  if ( self.id == update.player_id + "_selected_faction_lbl" )
                  {
                    self.UpdateText( update.update_txt );
                    self.UpdateBackground(
                      GetPrimaryFactionColor( update.update_txt )
                    );
                  }
                }
              ),
            TextLabel( player_id + "_readied" )
              .SetText( "Not Ready", 24 )
              .Background( RED )
              .On(
                InterfaceUpdate::ID::PlayerToggledReady,
                []( Element &self, InterfaceUpdate::Update update ) {
                  if ( self.id == update.player_id + "_readied" )
                  {
                    if ( update.condition )
                    {
                      self.UpdateBackground( GREEN );
                      self.UpdateText( "Ready" );
                    }
                    else
                    {
                      self.UpdateBackground( RED );
                      self.UpdateText( "Not Ready" );
                    }
                  }
                }
              ),
            TextLabel( player_id + "_steam_user_name" )
              .SetText( "", 24 )
              .Background( GRAY ),
          } );

      panel.Enable();

      slots.push_back( panel );
    }

    return slots;
  }

  inline std::vector<Element> CreateLobbyUI()
  {
    return {
      Panel( "lobby" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Children( {
          TextLabel( "lobby_title" )
            .SetText(
              SteamMatchmaking()->GetLobbyData( Network::lobby_id, "name" ), 32
            )
            .Background( GREEN ),
          Panel( "lobby_members_1_4" )
            .Children( CreateSlots( 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 ) )
            ),
          Panel( "lobby_members_5_8" )
            .Children( CreateSlots(
              Network::MAX_PLAYERS_PER_SERVER / 2,
              Network::MAX_PLAYERS_PER_SERVER
            ) ),
          TextButton( "lobby_back_to_main" )
            .SetText( "Back", 32 )
            .Background( RED )
            .SetEvent( InterfaceEvent::ID::ReturnToMain ),
          TextButton( "ready_up" )
            .SetText( "Ready Up", 32 )
            .Background( RED )
            .SetEvent( InterfaceEvent::ID::PlayerToggledReady ),
          TextButton( "start_game" )
            .StartDisabled()
            .Clickable( false )
            .SetText( "Start Game", 32 )
            .Background( RED )
            .SetEvent( InterfaceEvent::ID::HostStartGame )
            .On(
              InterfaceUpdate::ID::HostLobby,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.Enable();
              }
            )
            .On(
              InterfaceUpdate::ID::JoinLobby,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.Disable();
              }
            )
            .On(
              InterfaceUpdate::ID::AllPlayersReady,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.UpdateClickable( update.condition );
                if ( update.condition )
                  self.UpdateBackground( GREEN );
                else
                  self.UpdateBackground( RED );
              }
            ),
        } ),
    };
  }
};// namespace UI
