//
// Created by brett on 2/3/2022.
//

#pragma once

#include "../../../common.hpp"
#include "../../../resource.hpp"
#include "../../../state.hpp"
#include "provinces.hpp"
#include "terrain.hpp"

namespace Map {
  enum Mode {
    TERRAIN,
    POLITICAL,
  };

  inline Mode mode = Mode::TERRAIN;

  inline void Draw(State &state, entt::registry &reg, TextureCache &cache) {
    switch (mode) {
      case Mode::TERRAIN:
        Terrain::Draw(state.camera, reg, cache);
        Provinces::DrawProvinces(reg, cache, false);
        break;
      case Mode::POLITICAL:
        Terrain::Draw(state.camera, reg, cache);
        Provinces::DrawProvinces(reg, cache, true);
        break;
    }
  }

};// namespace Map
