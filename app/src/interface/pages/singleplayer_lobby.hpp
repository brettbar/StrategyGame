
#pragma once

#include "../../shared/common.hpp"

#include "../immediate.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"
#include <raylib.h>

namespace UI {

  enum class Action_SinglePlayerLobby {
    None,
    SelectFaction,
    ExitPressed,
  };

  inline Action_SinglePlayerLobby DrawSinglePlayerLobby() {
    auto f = Iron::Forge();

    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f.Grid( root, 3, 3 );

    auto grid = f.Grid( root_grid->Slot( 4 ), 1, 5 );

    bool select_faction =
      f.TextButton( grid->Slot( 0 ), "Select Faction", BLUE );

    f.TextLabel( grid->Slot( 1 ), "Waiting for faction", BLUE );


    bool exit_pressed = f.TextButton( grid->Slot( 4 ), "Return to Main", BLUE );

    if ( select_faction ) {
      return Action_SinglePlayerLobby::SelectFaction;
    }

    if ( exit_pressed ) {
      return Action_SinglePlayerLobby::ExitPressed;
    }

    f.Draw();

    return Action_SinglePlayerLobby::None;
  }

};// namespace UI
