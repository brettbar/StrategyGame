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
      auto lobbies = Network::Client()->GetLobbyList();

      for ( auto lobby_id: lobbies ) {
        const char *lobby_name =
          SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

        std::string id = "lobby_entry_" + std::string( lobby_name );

        if ( !Manager()->lookup.contains( id ) ) {
          children.push_back( Create<TextButton>(
            { id, std::string( lobby_name ), 24, GREEN, WHITE, true, true }
          ) );
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
