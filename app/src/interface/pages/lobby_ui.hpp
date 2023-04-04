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
            TextButton( player_id + "_faction_selection" )
              .SetText( "Select Faction", 24 )
              .Background( GRAY )
              .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage )
              .Clickable( false )
              .ListensFor( {
                InterfaceUpdate::ID::HostLobby,
                InterfaceUpdate::ID::JoinLobby,
              } ),
            TextLabel( player_id + "_select_faction" )
              .SetText( "Selecting Faction...", 24 )
              .Background( GRAY )
              .ListensFor( { InterfaceUpdate::ID::FactionSelected } ),
            TextLabel( player_id + "_label" )
              .SetText( "Open Slot " + std::to_string( i + 1 ), 24 )
              .Background( GRAY ),
            TextLabel( player_id + "_steam_user_name" )
              .SetText( "", 24 )
              .Background( GRAY ),
          } );
      panel.Enable();

      slots.push_back( panel );
    }

    return slots;
  }

  inline std::vector<Network::PeerData> GetPeers()
  {
    std::vector<Network::PeerData> members = {};

    if ( Network::is_host )
    {
      members = Network::Host()->GetConnectedUsers();
    }
    else
    {
      members = Network::Client()->GetConnectedUsers();
    }

    return members;
  };


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
            .SetEvent( InterfaceEvent::ID::ReadyUp )
            .ListensFor(
              { InterfaceUpdate::ID::HostLobby, InterfaceUpdate::ID::JoinLobby }
            ),
        } ),
    };
  }
};// namespace UI
