#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI {
  inline entt::entity
  CreateMemberPanel( std::string id, std::string member, bool is_host ) {
    std::string label = "something wrong";
    Color color = RED;

    if ( is_host ) {
      label = "Host: " + member;
      color = ORANGE;
    }
    else
      label = "Guest: " + member;

    entt::entity panel = Create<Panel>( {
      id,
      BLACK,
      Axis::Column,
      Align::Start,
      Align::Start,
      { Create<TextLabel>( { id + "_label", label, 24, color, WHITE, false }
      ) },
    } );

    RecursiveToggle( panel, true );

    return panel;
  }

  inline std::vector<entt::entity> CreateOpenSlots( u32 start, u32 end ) {
    std::vector<entt::entity> open_slots = {};

    std::string label = "Open Slot";

    for ( u32 i = start; i < end; i++ ) {
      std::string id = "open_slot_" + std::to_string( i );

      entt::entity panel = Create<Panel>( {
        id,
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        { Create<TextLabel>( { id + "_label", label, 24, GRAY, WHITE, false }
        ) },
      } );

      open_slots.push_back( panel );
      RecursiveToggle( panel, true );
    }


    return open_slots;
  }


  inline std::vector<entt::entity> CreateLobbyUI() {

    // TODO better way of making the id and label
    auto update_children = [](
                             std::vector<entt::entity> &children,
                             u32 start,
                             u32 end
                           ) {
      std::vector<std::string> members = {};

      if ( Network::is_host ) {
        members = Network::Host()->GetConnectedUsers();
      }
      else {
        // TODO client get connected users
        members = Network::Client()->GetConnectedUsers();
      }

      // for ( u32 i = start; i < end; i++ ) {

      //   if ( i >= members.size() ) {
      //     // TODO put in steam user in
      //     std::string id = "Open Slot: " + std::to_string( i );

      //     if ( !Manager()->lookup.contains( std::string( id ) ) ) {
      //       children.push_back( CreateOpenSlot( id ) );
      //     }
      //   }
      //   else {
      //     // TODO put in steam user in
      //     std::string id = "lobby_member_" + std::to_string( i );

      //     if ( !Manager()->lookup.contains( std::string( id ) ) ) {
      //       children.push_back( CreateMemberPanel( id, members[i], i == 0 ) );
      //     }
      //   }
      // }
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
          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;
        },
        {
          Create<TextLabel>( {
            "lobby_title",
            "",
            32,
            GREEN,
            WHITE,
            true,
            []() -> std::string {
              return SteamMatchmaking()->GetLobbyData(
                Network::lobby_id, "name"
              );
            },
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
        },
      } ),
    };
  }
};// namespace UI
