#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"


namespace UI {

  inline std::vector<entt::entity> CreateLobbyBrowser() {
    // TODO better way of making the id and label
    auto children = []() -> std::vector<entt::entity> {
      std::vector<entt::entity> lobby_buttons = {};
      auto lobbies = Network::Client()->GetLobbyList();

      for ( auto lobby_id: lobbies ) {
        const char *lobby_name =
          SteamMatchmaking()->GetLobbyData( lobby_id, "name" );


        lobby_buttons.push_back( TextButton::Create(
          "lobby_entry_" + std::string( lobby_name ),
          std::string( lobby_name ),
          16,
          GREEN,
          WHITE,
          true,
          true,
          [lobby_id]() {
            if ( Network::Client()->AttemptJoinLobby( lobby_id ) ) {}
          }
        ) );
      }

      return lobby_buttons;
    };

    return {
      Panel::CreateAbsolute(
        "lobby_browser",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        [children]( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;

          self.children = children();
        },
        {}
      ),
    };
  }
};// namespace UI
