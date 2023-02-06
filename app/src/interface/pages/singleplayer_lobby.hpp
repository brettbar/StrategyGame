
#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

namespace UI {
  inline std::vector<entt::entity> CreateSinglePlayerLobby() {
    return {
      Create<Panel>( {
        "singleplayer_lobby",
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
          Create<TextButton>( {
            "singleplayer_faction_label",
            "Select your faction",
            32,
            GREEN,
            WHITE,
            false,
            {},
            {},
          } ),
          Create<TextLabel>( {
            "singleplayer_faction_selected",
            "Waiting to Select Faction",
            32,
            GRAY,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "singleplayer_lobby_start_game",
            "Start Game",
            32,
            BLUE,
            WHITE,
            false,
            {},
            {},
          } ),
        },
      } ),
    };
  }
};// namespace UI
