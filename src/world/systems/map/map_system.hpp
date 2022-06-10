//
// Created by brett on 2/3/2022.
//

#pragma once

#include "../../../common.hpp"
#include "../../../renderer/textures.hpp"
#include "../../../state.hpp"
#include "province_system.hpp"
#include "terrain_system.hpp"

namespace MapSystem {
enum Mode {
  TERRAIN,
  POLITICAL,
};

inline Mode mode = Mode::TERRAIN;

inline void
Draw( State &state, entt::registry &reg, TextureCache &cache ) {
  switch ( mode ) {
    case Mode::TERRAIN:
      Terrain::Draw( state.camera, reg, cache );
      ProvinceSystem::DrawProvinces( reg, cache, false );
      break;
    case Mode::POLITICAL:
      Terrain::Draw( state.camera, reg, cache );
      ProvinceSystem::DrawProvinces( reg, cache, true );
      break;
  }
}

};// namespace MapSystem
