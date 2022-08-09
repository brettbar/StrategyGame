//
// Created by brett on 1/13/2022.
//

#pragma once

#include "common.hpp"
#include "global.hpp"
#include "renderer/textures.hpp"
#include "state.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"
#include "systems/map/map_system.hpp"
#include "systems/movement_system.hpp"
#include "systems/selection_system.hpp"
#include "systems/spawn_system.hpp"
#include "systems/ui/game_ui_system.hpp"
#include "save.hpp"
#include <raylib.h>

namespace Input {

inline void CheckMenuToggle() {
  if ( IsKeyPressed( KEY_CAPS_LOCK ) ) {
    if ( Global::program_mode == ProgramMode::GAME ) {
      Global::program_mode = ProgramMode::MODAL_MENU;
    } else if ( Global::program_mode == ProgramMode::MODAL_MENU ) {
      Global::program_mode = ProgramMode::GAME;
    }
  }
}

inline void Handle( State &state, entt::registry &reg, TextureCache &cache ) {
  Vector2 clickPos = GetScreenToWorld2D( GetMousePosition(), state.camera );


  if ( IsKeyDown( KEY_LEFT_CONTROL ) ) {
    if ( IsKeyPressed( KEY_S ) ) {
      Save::Save( reg );
    }

    if ( IsKeyPressed( KEY_L ) ) {
      Save::Load();
    }
  }

  if ( IsKeyPressed( KEY_SPACE ) ) {
    if ( state.timeScale > 0.0f ) {
      state.prevTimeScale = state.timeScale;
      state.timeScale = 0.0f;
    } else if ( state.timeScale == 0.0f ) {
      state.timeScale = state.prevTimeScale;
    }
  }

  if ( IsKeyPressed( KEY_MINUS ) ) {
    state.timeScale -= 0.5f;
    if ( state.timeScale < 0.0f )
      state.timeScale = 0.0f;

    if ( state.timeScale == 0.0f && state.prevTimeScale > 0.5f ) {
      state.prevTimeScale -= 0.5f;
      state.timeScale = state.prevTimeScale;
    }
  }

  if ( IsKeyPressed( KEY_EQUAL ) ) {
    state.timeScale += 0.5f;
    if ( state.timeScale > 1.5f )
      state.timeScale = 1.5f;
  }

  if ( IsKeyPressed( KEY_V ) ) {
    SpawnSystem::CreateNew( reg, cache, clickPos, state.currPlayer );
  }

  if ( IsKeyPressed( KEY_C ) ) {
    ProvinceSystem::AssignProvince( reg, state.currPlayer->id, clickPos );
  }

  if ( IsKeyPressed( KEY_GRAVE ) ) {
    if ( state.gameState == GameState::GAME )
      state.gameState = GameState::EDITOR;
    else if ( state.gameState == GameState::EDITOR )
      state.gameState = GameState::GAME;
  }

  if ( IsMouseButtonPressed( 0 ) ) {
    // IRONGUI::Input(state, reg);
    // if ( !IRONGUI::HandleMouseEvent( 0, GetMousePosition() ) )
    if ( !IRONGUI::MouseIsOverUI() )
      SelectionSystem::UpdateSelection( reg, clickPos );
  }
  if ( IsMouseButtonPressed( 1 ) ) {
    // if ( !IRONGUI::HandleMouseEvent( 1, GetMousePosition() ) )
    if ( !IRONGUI::MouseIsOverUI() )
      MovementSystem::SetDestinations( reg, state.camera );
  }

  if ( IsKeyPressed( KEY_ONE ) ) {
    state.currPlayer->id = 0;
    state.currPlayer->faction = ROMANS;
    state.currPlayer->factionName = "Roman Republic";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_TWO ) ) {
    state.currPlayer->id = 1;
    state.currPlayer->faction = GREEKS;
    state.currPlayer->factionName = "Greek Cities";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_THREE ) ) {
    state.currPlayer->id = 2;
    state.currPlayer->faction = CELTS;
    state.currPlayer->factionName = "Celtic Tribes";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_FOUR ) ) {
    state.currPlayer->id = 3;
    state.currPlayer->faction = PUNICS;
    state.currPlayer->factionName = "Punic Colonies";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_FIVE ) ) {
    state.currPlayer->id = 4;
    state.currPlayer->faction = PERSIANS;
    state.currPlayer->factionName = "Persian Empire";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_P ) ) {
    MapSystem::mode = MapSystem::Mode::POLITICAL;
  }

  if ( IsKeyPressed( KEY_T ) ) {
    MapSystem::mode = MapSystem::Mode::TERRAIN;
  }
}
};// namespace Input
