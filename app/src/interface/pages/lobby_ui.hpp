#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_button.hpp"

#include "../ui_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI
{
  inline ptr<Panel> CreateMemberPanel(
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


    ptr<Panel> panel = Create<Panel>( {
      id,
      BLACK,
      Axis::Column,
      Align::Start,
      Align::Start,
      Margins{ 16, 16, 0, 0 },
      {
        Create<TextButton>( {
          id + "_faction_selection",
          "Select Faction",
          24,
          color,
          WHITE,
          false,
          InterfaceEvent::ID::OpenFactionSelectPage,
          { InterfaceUpdate::ID::JoinLobby },
        } ),
        // Create<TextureLabel>( { "romans_villager_texture" } ),
        Create<TextLabel>( {
          id + "_select_faction",
          "Selecting Faction...",
          24,
          color,
          WHITE,
          {
            InterfaceUpdate::ID::FactionSelected,
          },
        } ),
        Create<TextLabel>( {
          id + "_label",
          label,
          24,
          color,
          WHITE,
        } ),
        Create<TextLabel>( {
          id + "_steam_user_name",
          std::string( SteamFriends()
                         ->GetFriendPersonaName( member.steam_user_id ) ),
          24,
          color,
          WHITE,
        } ),
      },
    } );

    panel->Enable();

    return panel;
  }

  inline ptr<Panel> CreateOpenSlot( u32 i )
  {
    std::string id = "open_slot_" + std::to_string( i );

    ptr<Panel> panel = Create<Panel>( {
      id,
      BLACK,
      Axis::Column,
      Align::Start,
      Align::Start,
      Margins{ 16, 16, 0, 0 },
      { Create<TextLabel>( {
        id + "_label",
        "Open Slot " + std::to_string( i + 1 ),
        24,
        GRAY,
        WHITE,
      } ) },
    } );

    panel->Enable();

    return panel;
  }

  inline std::vector<ptr<Element>> CreateOpenSlots( u32 start, u32 end )
  {
    std::vector<ptr<Element>> open_slots = {};

    for ( u32 i = start; i < end; i++ )
    {
      open_slots.push_back( CreateOpenSlot( i ) );
    }

    return open_slots;
  }


  inline std::vector<ptr<Element>> CreateLobbyUI()
  {

    // TODO better way of making the id and label
    auto update_children =
      []( std::vector<ptr<Element>> &children, u32 start, u32 end ) {
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
              RecursiveDelete( Manager()->lookup[id] );
              children[i] = CreateOpenSlot( i );
            }
          }
          else
          {
            if ( !Manager()->lookup.contains( id ) )
            {
              RecursiveDelete(
                Manager()->lookup["open_slot_" + std::to_string( i )]
              );

              children[i] = CreateMemberPanel( i, id, members[i], i == 0 );
            }
          }
        }
      };

    return {
      Create<Panel>( {
        "lobby",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        true,
        []( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 400 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.transform.x = updated_pos.x;
          self.transform.y = updated_pos.y;
        },
        {
          Create<TextLabel>( {
            "lobby_title",
            SteamMatchmaking()->GetLobbyData( Network::lobby_id, "name" ),
            32,
            GREEN,
            WHITE,
          } ),
          Create<Panel>( {
            "lobby_members_1_4",
            BLACK,
            Axis::Row,
            Align::Start,
            Align::Start,
            true,
            [update_children]( Panel &self ) {
              update_children(
                self.children, 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 )
              );
            },
            CreateOpenSlots( 0, ( Network::MAX_PLAYERS_PER_SERVER / 2 ) ),
          } ),
          Create<Panel>( {
            "lobby_members_5_8",
            BLACK,
            Axis::Row,
            Align::Start,
            Align::Start,
            true,
            [update_children]( Panel &self ) {
              update_children(
                self.children,
                Network::MAX_PLAYERS_PER_SERVER / 2,
                Network::MAX_PLAYERS_PER_SERVER
              );
            },
            CreateOpenSlots(
              Network::MAX_PLAYERS_PER_SERVER / 2,
              Network::MAX_PLAYERS_PER_SERVER
            ),
          } ),
          Create<TextButton>( {
            "lobby_back_to_main",
            "Back",
            32,
            RED,
            WHITE,
            InterfaceEvent::ID::ReturnToMain,
          } ),
          Create<TextButton>( {
            "ready_up",
            "Ready Up",
            32,
            RED,
            WHITE,
            InterfaceEvent::ID::ReadyUp,
            { InterfaceUpdate::ID::HostLobby, InterfaceUpdate::ID::JoinLobby },
          } ),
        },
      } ),
    };
  }
};// namespace UI
