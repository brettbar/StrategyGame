#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateLobbyUI() {

    // TODO better way of making the id and label
    auto children = []() -> std::vector<entt::entity> {
      std::vector<Network::ClientConnectionData> members = {};

      if ( Network::is_host ) {
        members = Network::Host()->GetConnectedUsers();
      }
      else {
        // TODO client get conencted users
        members = {};
      }


      auto labels = std::vector<entt::entity>() = { Create<TextLabel>(
        { "lobby_member_" + std::string( SteamFriends()->GetPersonaName() ),
          "User: " + std::string( SteamFriends()->GetPersonaName() ),
          32,
          PURPLE,
          WHITE,
          true }
      ) };

      for ( u32 i = 0; i < members.size(); i++ ) {
        // TODO put in steam user in
        std::string id = "lobby_member_" + std::to_string( i );

        // TODO put in steam user in
        std::string label =
          "User: " + std::string( SteamFriends()->GetFriendPersonaName(
                       members[i].steam_user_id
                     ) );


        labels.push_back( Create<TextLabel>( { id, label, 32, RED, WHITE, true }
        ) );
      }

      return labels;
    };

    return {
      Create<Panel>( {
        "lobby",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        true,
        [children]( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;

          self.children = children();
        },
        {},
      } ),
    };
  }
};// namespace UI
