#pragma once

#include "../../shared/common.hpp"

#include "../ui_builder.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI
{
  // inline Element CreateMemberPanel(
  //   u32 i,
  //   std::string player_id,
  //   Network::PeerData member,
  //   bool is_host
  // )
  // {
  //   std::string label = "something wrong";
  //   Color color = RED;

  //   if ( is_host )
  //   {
  //     label = "Host: " + member.player_id;
  //     color = ORANGE;
  //   }
  //   else
  //     label = "Guest: " + member.player_id;


  //   Element panel =
  //     Panel( player_id )
  //       .SetAxis( Axis::Column )
  //       .Margins( { 16, 16, 0, 0 } )
  //       .Children( {
  //         TextButton( player_id + "_faction_selection" )
  //           .SetText( "Select Faction", 24 )
  //           .Background( color )
  //           .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage )
  //           .Clickable( false )
  //           .ListensFor(
  //             { InterfaceUpdate::ID::HostLobby, InterfaceUpdate::ID::JoinLobby }
  //           )
  //           .build(),
  //         TextLabel( player_id + "_select_faction" )
  //           .SetText( "Selecting Faction...", 24 )
  //           .Background( color )
  //           .ListensFor( { InterfaceUpdate::ID::FactionSelected } )
  //           .build(),
  //         TextLabel( player_id + "_label" )
  //           .SetText( label, 24 )
  //           .Background( color )
  //           .build(),
  //         TextLabel( player_id + "_steam_user_name" )
  //           .SetText(
  //             std::string(
  //               SteamFriends()->GetFriendPersonaName( member.steam_user_id )
  //             ),
  //             24
  //           )
  //           .Background( color )
  //           .build(),
  //       } )
  //       .build();

  //   panel.Enable();

  //   return panel;
  // }


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
              } )
              .build(),
            TextLabel( player_id + "_select_faction" )
              .SetText( "Selecting Faction...", 24 )
              .Background( GRAY )
              .ListensFor( { InterfaceUpdate::ID::FactionSelected } )
              .build(),
            TextLabel( player_id + "_label" )
              .SetText( "Open Slot " + std::to_string( i + 1 ), 24 )
              .Background( GRAY )
              .build(),
            TextLabel( player_id + "_steam_user_name" )
              .SetText( "", 24 )
              .Background( GRAY )
              .build(),
          } )
          .build();
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

  // inline void UpdateRow( std::vector<Element> &children, u32 begin, u32 end )
  // {
  //   auto members = GetPeers();
  //   for ( u32 i = begin; i < end; i++ )
  //   {
  //     std::string player_id = "player_" + std::to_string( i );

  //     if ( i >= members.size() )
  //     {
  //       // We know the user is disconnected
  //       if ( Manager()->lookup.contains( player_id ) )
  //       {
  //         // Manager()->lookup[player_id]->Destroy();
  //         // children[i] = CreateOpenSlot( i );
  //       }
  //     }
  //     else
  //     {
  //       if ( !Manager()->lookup.contains( player_id ) )
  //       {
  //         Manager()->lookup["open_slot_" + std::to_string( i )]->Destroy();
  //         children[i] = CreateMemberPanel( i, player_id, members[i], i == 0 );
  //       }
  //     }
  //   }
  // }


  inline std::vector<Element> CreateLobbyUI()
  {
    auto update_first_row = []( std::vector<Element> &children ) {
      // UpdateRow( children, 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 ) );
    };

    auto update_second_row = []( std::vector<Element> &children ) {
      // UpdateRow(
      //   children,
      //   ( Network::MAX_PLAYERS_PER_SERVER / 2 ),
      //   Network::MAX_PLAYERS_PER_SERVER
      // );
    };

    return {
      Panel( "lobby" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Children( {
          TextLabel( "lobby_title" )
            .SetText(
              SteamMatchmaking()->GetLobbyData( Network::lobby_id, "name" ), 32
            )
            .Background( GREEN )
            .build(),
          Panel( "lobby_members_1_4" )
            .Children( CreateSlots( 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 ) )
            )
            .UpdateChildren( update_first_row )
            .build(),
          Panel( "lobby_members_5_8" )
            .Children( CreateSlots(
              Network::MAX_PLAYERS_PER_SERVER / 2,
              Network::MAX_PLAYERS_PER_SERVER
            ) )
            .UpdateChildren( update_second_row )
            .build(),
          TextButton( "lobby_back_to_main" )
            .SetText( "Back", 32 )
            .Background( RED )
            .SetEvent( InterfaceEvent::ID::ReturnToMain )
            .build(),
          TextButton( "ready_up" )
            .SetText( "Ready Up", 32 )
            .Background( RED )
            .SetEvent( InterfaceEvent::ID::ReadyUp )
            .ListensFor(
              { InterfaceUpdate::ID::HostLobby, InterfaceUpdate::ID::JoinLobby }
            )
            .build(),
        } )
        .build(),
    };
  }
};// namespace UI
