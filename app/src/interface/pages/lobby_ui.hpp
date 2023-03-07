#pragma once

#include "../../shared/common.hpp"

#include "../ui_builder.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI
{
  inline Element CreateMemberPanel(
    u32 i,
    std::string id,
    Network::PeerData member,
    bool is_host
  )
  {
    std::string label = "something wrong";
    Color color = RED;

    if ( is_host )
    {
      label = "Host: " + member.player_id;
      color = ORANGE;
    }
    else
      label = "Guest: " + member.player_id;


    Element panel =
      Panel( id )
        .SetAxis( Axis::Column )
        .Margins( { 16, 16, 0, 0 } )
        .Children( {
          TextButton( id + "_faction_selection" )
            .SetText( "Select Faction", 24 )
            .Background( color )
            .SetEvent( InterfaceEvent::ID::OpenFactionSelectPage )
            .ListensFor( { InterfaceUpdate::ID::JoinLobby } )
            .build(),
          TextLabel( id + "_select_faction" )
            .SetText( "Selecting Faction...", 24 )
            .Background( color )
            .ListensFor( { InterfaceUpdate::ID::FactionSelected } )
            .build(),
          TextLabel( id + "_label" )
            .SetText( label, 24 )
            .Background( color )
            .build(),
          TextLabel( id + "_steam_user_name" )
            .SetText(
              std::string(
                SteamFriends()->GetFriendPersonaName( member.steam_user_id )
              ),
              24
            )
            .Background( color )
            .build(),
        } )
        .build();

    panel.Enable();

    return panel;
  }

  inline Element CreateOpenSlot( u32 i )
  {
    std::string id = "open_slot_" + std::to_string( i );

    Element panel = Panel( id )
                      .SetAxis( Axis::Column )
                      .Margins( { 16, 16, 0, 0 } )
                      .Children( {
                        TextLabel( id + "_label" )
                          .SetText( "Open Slot " + std::to_string( i + 1 ), 24 )
                          .Background( GRAY )
                          .build(),
                      } )
                      .build();
    panel.Enable();

    return panel;
  }

  inline std::vector<Element> CreateOpenSlots( u32 start, u32 end )
  {
    std::vector<Element> open_slots = {};

    for ( u32 i = start; i < end; i++ )
    {
      open_slots.push_back( CreateOpenSlot( i ) );
    }

    return open_slots;
  }


  inline std::vector<Element> CreateLobbyUI()
  {

    // TODO better way of making the id and label
    auto update_children =
      []( std::vector<Element> &children, u32 start, u32 end ) {
        std::vector<Network::PeerData> members = {};

        if ( Network::is_host )
        {
          members = Network::Host()->GetConnectedUsers();
        }
        else
        {
          members = Network::Client()->GetConnectedUsers();
        }

        for ( u32 i = start; i < end; i++ )
        {
          std::string id = "player_" + std::to_string( i );

          if ( i >= members.size() )
          {
            // We know the user is disconnected
            if ( Manager()->lookup.contains( id ) )
            {
              Manager()->lookup[id]->Destroy();
              children[i] = CreateOpenSlot( i );
            }
          }
          else
          {
            if ( !Manager()->lookup.contains( id ) )
            {
              Manager()->lookup["open_slot_" + std::to_string( i )]->Destroy();
              children[i] = CreateMemberPanel( i, id, members[i], i == 0 );
            }
          }
        }
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
            .Children(
              CreateOpenSlots( 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 ) )
            )
            .UpdateSubsetChildren(
              update_children, 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 )
            )
            .build(),
          Panel( "lobby_members_5_8" )
            .Children( CreateOpenSlots(
              Network::MAX_PLAYERS_PER_SERVER / 2,
              Network::MAX_PLAYERS_PER_SERVER
            ) )
            .UpdateSubsetChildren(
              update_children,
              Network::MAX_PLAYERS_PER_SERVER / 2,
              Network::MAX_PLAYERS_PER_SERVER
            )
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
