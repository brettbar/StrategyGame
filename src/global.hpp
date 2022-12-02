#pragma once

#include "renderer/fonts.hpp"
#include "renderer/textures.hpp"
#include "state.hpp"

namespace Global {

enum class ProgramMode {
  MainMenu,
  ModalMenu,
  Game,
  Editor,
};

// TODO delete?
enum class GameMode {
  CAMPAIGN,
  BATTLE,
};

inline State state = {
  .mapWidth = 128,
  .mapHeight = 128,
  .timeScale = 0.0f,
  .prevTimeScale = 1.0f,
  .gameState = GameState::EDITOR,
  .month = 1,
  .year = 4,
  .startYear = 4,
  // .currPlayer = std::make_shared<TempPS>( TempPS( 0, Faction::ROMANS ) ),
};

inline ProgramMode program_mode = ProgramMode::Game;
inline FontCache font_cache = {};
inline TextureCache texture_cache = {};
inline entt::registry world;
inline entt::registry local;


inline bool game_started = false;
inline bool host_mode = true;

// TODO temporary
inline entt::entity host_player;

};// namespace Global
