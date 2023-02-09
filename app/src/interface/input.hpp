//
// Created by brett on 1/13/2022.
//


#pragma once

#include <raylib.h>

#include "../shared/common.hpp"
#include "../shared/global.hpp"
#include "../shared/save.hpp"
#include "../shared/state.hpp"
#include "../shared/textures.hpp"

#include "../world/managers/commands.hpp"

#include "../world/components/sight.hpp"
#include "../world/components/unit.hpp"
#include "../world/systems/map_system.hpp"
#include "../world/systems/movement_system.hpp"
#include "../world/systems/selection_system.hpp"

#include "ui_system.hpp"

namespace Input {

  inline void CheckMenuToggle() {
    if ( IsKeyPressed( KEY_CAPS_LOCK ) || IsKeyPressed( KEY_ESCAPE ) ) {
      Events::event_emitter.publish( Events::Basic{ "toggle_modal_menu" } );
    }
  }


  inline void Handle() {
    Vector2 click_pos =
      GetScreenToWorld2D( GetMousePosition(), Global::state.camera );

    if ( IsKeyPressed( KEY_SPACE ) ) {
      Commands::Manager()->PostCommand(
        Commands::Type::TimeChange, "Player request Pause"
      );
    }

    if ( IsKeyPressed( KEY_MINUS ) ) {
      Commands::Manager()->PostCommand(
        Commands::Type::TimeChange, "Player request Slower"
      );
    }

    if ( IsKeyPressed( KEY_EQUAL ) ) {
      Commands::Manager()->PostCommand(
        Commands::Type::TimeChange, "Player request Faster"
      );
    }

    if ( IsKeyPressed( KEY_V ) ) {
      Commands::Manager()->PostCommand(
        Commands::Type::Spawn, "Player spawn Villager", click_pos
      );
    }

    if ( IsKeyPressed( KEY_C ) ) {
      Commands::Manager()->PostCommand(
        Commands::Type::Spawn, "Player spawn City", click_pos
      );
    }

    if ( IsMouseButtonPressed( 0 ) ) {
      if ( !UI::Manager()->MouseIsOverUI() )
        Commands::Manager()->PostCommand(
          Commands::Type::Selection, "Player select", click_pos
        );
    }

    if ( IsMouseButtonPressed( 1 ) ) {
      if ( !UI::Manager()->MouseIsOverUI() )
        Commands::Manager()->PostCommand( Commands::Type::Move, "Player move" );
    }

    if ( IsKeyPressed( KEY_P ) ) {
      MapSystem::mode = MapSystem::Mode::Political;
    }

    if ( IsKeyPressed( KEY_T ) ) {
      MapSystem::mode = MapSystem::Mode::Terrain;
    }
  }
};// namespace Input
