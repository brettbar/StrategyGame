#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"


namespace UI {
  inline std::vector<entt::entity> CreateLobbyBrowser() {

    // TODO better way of making the id and label
    auto update_children = []( std::vector<entt::entity> &children ) {
      for ( CSteamID lobby_id: Network::Client()->GetLobbyList() ) {

        const char *lobby_name =
          SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

        if ( lobby_name && lobby_name[0] && !Manager()->lookup.contains( std::string( lobby_name ) ) ) {
          std::cout << "Lobby Name: " << lobby_name << std::endl;

          std::string button_id = "lobby_entry_" + std::string( lobby_name );

          std::function<void()> action = [button_id, lobby_id]() {
            Events::event_emitter.publish( Events::JoinLobby{
              .origin_id = button_id,
              .lobby_id = lobby_id,
            } );
          };


          // TODO only clickable based on host/client checksum compat
          entt::entity button_e = Create<TextButton>( {
            lobby_name,
            std::string( lobby_name ),
            24,
            GREEN,
            WHITE,
            false,
            action,
          } );

          TextButton &button = Manager()->registry.get<TextButton>( button_e );
          button.elem.Enable();

          children.push_back( button_e );
        }
      }

      // TODO remove the buttons that dont have a lobby anymore
    };

    return {
      Create<Panel>( {
        "lobby_browser",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        true,
        [update_children]( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;

          update_children( self.children );
        },
        {},
      } ),
    };
  }
};// namespace UI
