//
// Created by brett on 1/13/2022.
//

// TODO this needs a huge refactor

#pragma once

#include "../ui/ui_system.hpp"
#include "../world/components/sight.hpp"
#include "../world/components/unit.hpp"
#include "../world/systems/map_system.hpp"
#include "../world/systems/movement_system.hpp"
#include "../world/systems/province_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/spawn_system.hpp"
#include "common.hpp"
#include "global.hpp"
#include "save.hpp"
#include "state.hpp"
#include "textures.hpp"
#include <raylib.h>

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
    if ( Global::state.timeScale > 0.0f ) {
      Global::state.prevTimeScale = Global::state.timeScale;
      Global::state.timeScale = 0.0f;
    }
    else if ( Global::state.timeScale == 0.0f ) {
      Global::state.timeScale = Global::state.prevTimeScale;
    }
  }

  if ( IsKeyPressed( KEY_MINUS ) ) {
    Global::state.timeScale -= 0.5f;
    if ( Global::state.timeScale < 0.0f )
      Global::state.timeScale = 0.0f;

    if ( Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f ) {
      Global::state.prevTimeScale -= 0.5f;
      Global::state.timeScale = Global::state.prevTimeScale;
    }
  }

  if ( IsKeyPressed( KEY_EQUAL ) ) {
    Global::state.timeScale += 0.5f;
    if ( Global::state.timeScale > 1.5f )
      Global::state.timeScale = 1.5f;
  }

  if ( IsKeyPressed( KEY_V ) ) {
    SpawnSystem::SpawnColonist( Global::host_player, click_pos );
  }

  if ( IsKeyPressed( KEY_C ) ) {
    // TODO this is hard coded to 0 since the player is 0
    ProvinceSystem::AssignProvince( Global::host_player, click_pos );
  }

  if ( IsKeyPressed( KEY_GRAVE ) ) {
    if ( Global::state.gameState == GameState::GAME )
      Global::state.gameState = GameState::EDITOR;
    else if ( Global::state.gameState == GameState::EDITOR )
      Global::state.gameState = GameState::GAME;
  }

  if ( IsMouseButtonPressed( 0 ) ) {
    if ( !UI::MouseIsOverUI() )
      SelectionSystem::UpdateSelection( click_pos );
  }
  if ( IsMouseButtonPressed( 1 ) ) {
    if ( !UI::MouseIsOverUI() )
      MovementSystem::SetDestinations( Global::state.camera );
  }

  // if ( IsKeyPressed( KEY_ONE ) ) {
  //   state.currPlayer->id = 0;
  //   state.currPlayer->faction = Faction::ROMANS;
  //   state.currPlayer->RefreshTextureMap();
  // }

  // if ( IsKeyPressed( KEY_TWO ) ) {
  //   state.currPlayer->id = 1;
  //   state.currPlayer->faction = Faction::GREEKS;
  //   state.currPlayer->RefreshTextureMap();
  // }

  // if ( IsKeyPressed( KEY_THREE ) ) {
  //   state.currPlayer->id = 2;
  //   state.currPlayer->faction = Faction::CELTS;
  //   state.currPlayer->RefreshTextureMap();
  // }

  // if ( IsKeyPressed( KEY_FOUR ) ) {
  //   state.currPlayer->id = 3;
  //   state.currPlayer->faction = Faction::PUNICS;
  //   state.currPlayer->RefreshTextureMap();
  // }

  // if ( IsKeyPressed( KEY_FIVE ) ) {
  //   state.currPlayer->id = 4;
  //   state.currPlayer->faction = Faction::PERSIANS;
  //   state.currPlayer->RefreshTextureMap();
  // }

  if ( IsKeyPressed( KEY_P ) ) {
    MapSystem::mode = MapSystem::Mode::Political;
  }

  if ( IsKeyPressed( KEY_T ) ) {
    MapSystem::mode = MapSystem::Mode::Terrain;
  }
}
};// namespace Input
