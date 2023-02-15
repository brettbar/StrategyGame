// The goal is to slowly move everything out of this file and eventually delete it

#pragma once

#include "fonts.hpp"
#include "state.hpp"
#include "textures.hpp"


namespace Global {

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

  inline FontCache font_cache = {};
  inline TextureCache texture_cache = {};
  inline entt::registry world;

  // TODO temporary
  inline entt::entity host_player;

  inline void ClearRegistry() {
    Global::world = {};
    Global::world.clear();
  }


};// namespace Global
