//
// Created by brett on 1/13/2022.
//

#pragma once

#include "common.hpp"
#include "guilib/gui_system.hpp"
#include "resource.hpp"
#include "state.hpp"
#include "world/systems/map/map_system.hpp"
#include "world/systems/map/province_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/selection_system.hpp"
#include "world/systems/spawn_system.hpp"
#include "world/systems/ui/ui_system.hpp"

namespace Input {
inline void Handle( State &state, entt::registry &reg, TextureCache &cache ) {
  Vector2 clickPos = GetScreenToWorld2D( GetMousePosition(), state.camera );

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
    ProvinceSystem::SetProvinceOwner( reg, state.currPlayer->id, clickPos );
  }

  if ( IsKeyPressed( KEY_GRAVE ) ) {
    if ( state.gameState == GameState::GAME )
      state.gameState = GameState::EDITOR;
    else if ( state.gameState == GameState::EDITOR )
      state.gameState = GameState::GAME;
  }

  if ( IsMouseButtonPressed( 0 ) ) {
    // UI::Input(state, reg);
    // if ( !UI::HandleMouseEvent( 0, GetMousePosition() ) )
    if ( !GUI::MouseWasOverUI() )
      SelectionSystem::UpdateSelection( reg, clickPos );
  }
  if ( IsMouseButtonPressed( 1 ) ) {
    // if ( !UI::HandleMouseEvent( 1, GetMousePosition() ) )
    if ( !GUI::MouseWasOverUI() )
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
