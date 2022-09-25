//
// Created by brett on 2/3/2022.
//

#pragma once

#include "../../common.hpp"
#include "../../renderer/textures.hpp"
#include "../../state.hpp"
#include "province_system.hpp"
#include "terrain_system.hpp"

namespace MapSystem {
enum class Mode {
  TERRAIN,
  POLITICAL,
};

inline Mode mode = Mode::TERRAIN;

};// namespace MapSystem
