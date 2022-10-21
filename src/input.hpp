//
// Created by brett on 1/13/2022.
//

#pragma once

#include "common.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"
#include "global.hpp"
#include "renderer/textures.hpp"
#include "save.hpp"
#include "state.hpp"
#include "systems/map/map_system.hpp"
#include "systems/movement_system.hpp"
#include "systems/selection_system.hpp"
#include "systems/spawn_system.hpp"
#include "ui/ui_system.hpp"
#include <raylib.h>

namespace Input {

inline void CheckMenuToggle() {
  if ( IsKeyPressed( KEY_CAPS_LOCK ) ) {
    if ( Global::program_mode == Global::ProgramMode::GAME ) {
      Global::program_mode = Global::ProgramMode::MODAL_MENU;
    } else if ( Global::program_mode == Global::ProgramMode::MODAL_MENU ) {
      Global::program_mode = Global::ProgramMode::GAME;
    }
  }
}

inline void Handle( State &state, entt::registry &reg, TextureCache &cache ) {
  Vector2 click_pos = GetScreenToWorld2D( GetMousePosition(), state.camera );


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
    SpawnSystem::CreateNew( cache, click_pos, state.currPlayer );
  }

  if ( IsKeyPressed( KEY_C ) ) {
    ProvinceSystem::AssignProvince( reg, state.currPlayer->id, click_pos );
  }

  if ( IsKeyPressed( KEY_GRAVE ) ) {
    if ( state.gameState == GameState::GAME )
      state.gameState = GameState::EDITOR;
    else if ( state.gameState == GameState::EDITOR )
      state.gameState = GameState::GAME;
  }

  if ( IsMouseButtonPressed( 0 ) ) {
    // IRONGUI::Input(state, reg);
    if ( !UI::MouseIsOverUI() )
      SelectionSystem::UpdateSelection( reg, click_pos );
  }
  if ( IsMouseButtonPressed( 1 ) ) {
    if ( !UI::MouseIsOverUI() ) {
      MovementSystem::SetDestinations( reg, state.camera );
      SelectionSystem::ClearSelection( reg, reg.view<Province::Component>() );
    }
  }

  if ( IsKeyPressed( KEY_ONE ) ) {
    state.currPlayer->id = 0;
    state.currPlayer->faction = Faction::ROMANS;
    state.currPlayer->factionName = "Roman Republic";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_TWO ) ) {
    state.currPlayer->id = 1;
    state.currPlayer->faction = Faction::GREEKS;
    state.currPlayer->factionName = "Greek Cities";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_THREE ) ) {
    state.currPlayer->id = 2;
    state.currPlayer->faction = Faction::CELTS;
    state.currPlayer->factionName = "Celtic Tribes";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_FOUR ) ) {
    state.currPlayer->id = 3;
    state.currPlayer->faction = Faction::PUNICS;
    state.currPlayer->factionName = "Punic Colonies";
    state.currPlayer->RefreshTextureMap();
  }

  if ( IsKeyPressed( KEY_FIVE ) ) {
    state.currPlayer->id = 4;
    state.currPlayer->faction = Faction::PERSIANS;
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
