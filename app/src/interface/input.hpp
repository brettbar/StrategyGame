//
// Created by brett on 1/13/2022.
//


#pragma once

#include <raylib.h>

#include "../shared/commands.hpp"
#include "../shared/common.hpp"
#include "../shared/global.hpp"
#include "../shared/save.hpp"
#include "../shared/state.hpp"
#include "../shared/textures.hpp"
#include "../world/components/sight.hpp"
#include "../world/components/unit.hpp"
#include "../world/systems/map_system.hpp"
#include "../world/systems/movement_system.hpp"
#include "../world/systems/selection_system.hpp"

#include "ui_system.hpp"

namespace Input {

inline void CheckMenuToggle() {
  if ( IsKeyPressed( KEY_CAPS_LOCK ) || IsKeyPressed( KEY_ESCAPE ) ) {
    if ( Global::program_mode == Global::ProgramMode::Campaign ) {
      Global::program_mode = Global::ProgramMode::ModalMenu;
      UI::EnableModalMenuUI();
    }
    else if ( Global::program_mode == Global::ProgramMode::ModalMenu ) {
      Global::program_mode = Global::ProgramMode::Campaign;
      UI::EnableCampaignUI();
    }
  }
}

inline void Handle() {
  Vector2 click_pos =
    GetScreenToWorld2D( GetMousePosition(), Global::state.camera );

  if ( IsKeyPressed( KEY_SPACE ) ) {
    Commands::queue.Enqueue( Commands::Command{
      Commands::Type::TimeChange,
      "Player request Pause",
    } );
  }

  if ( IsKeyPressed( KEY_MINUS ) ) {
    Commands::queue.Enqueue( Commands::Command{
      Commands::Type::TimeChange,
      "Player request Slower",
    } );
  }

  if ( IsKeyPressed( KEY_EQUAL ) ) {
    Commands::queue.Enqueue( Commands::Command{
      Commands::Type::TimeChange,
      "Player request Faster",
    } );
  }

  if ( IsKeyPressed( KEY_V ) ) {
    Commands::queue.Enqueue( Commands::Command{
      Commands::Type::Spawn,
      "Player spawn Villager",
      click_pos,
    } );
  }

  if ( IsKeyPressed( KEY_C ) ) {
    Commands::queue.Enqueue( Commands::Command{
      Commands::Type::Spawn,
      "Player spawn City",
      click_pos,
    } );
  }

  // if ( IsKeyPressed( KEY_GRAVE ) ) {
  //   if ( Global::state.gameState == GameState::GAME )
  //     Global::state.gameState = GameState::EDITOR;
  //   else if ( Global::state.gameState == GameState::EDITOR )
  //     Global::state.gameState = GameState::GAME;
  // }

  if ( IsMouseButtonPressed( 0 ) ) {
    if ( !UI::MouseIsOverUI() )
      Commands::queue.Enqueue( Commands::Command{
        Commands::Type::Selection,
        "Player select",
        click_pos,
      } );
  }

  if ( IsMouseButtonPressed( 1 ) ) {
    if ( !UI::MouseIsOverUI() )
      Commands::queue.Enqueue( Commands::Command{
        Commands::Type::Move,
        "Player move",
      } );
  }

  if ( IsKeyPressed( KEY_P ) ) {
    MapSystem::mode = MapSystem::Mode::Political;
  }

  if ( IsKeyPressed( KEY_T ) ) {
    MapSystem::mode = MapSystem::Mode::Terrain;
  }
}
};// namespace Input
