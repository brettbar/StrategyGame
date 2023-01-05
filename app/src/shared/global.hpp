#pragma once

#include "fonts.hpp"
#include "state.hpp"
#include "textures.hpp"


namespace Global {

enum class ProgramMode {
  MainMenu,
  ModalMenu,
  Campaign,
  Editor,
};

inline State state = {
  .mapWidth = 128,
  .mapHeight = 128,
  .timeScale = 0.0f,
  .prevTimeScale = 1.0f,
  .month = 1,
  .year = 4,
  .startYear = 4,
  // .currPlayer = std::make_shared<TempPS>( TempPS( 0, Faction::ROMANS ) ),
};

inline ProgramMode program_mode = ProgramMode::MainMenu;
inline FontCache font_cache = {};
inline TextureCache texture_cache = {};
inline entt::registry world;
inline entt::registry local;

// TODO temporary
inline entt::entity host_player;

inline void ClearRegistry() {
  Global::world = {};
  Global::local = {};
  Global::world.clear();
  Global::local.clear();
}


};// namespace Global
