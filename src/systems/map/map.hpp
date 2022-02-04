//
// Created by brett on 2/3/2022.
//

#pragma once

#include "../../common.hpp"
#include "../../resource.hpp"
#include "../../state.hpp"
#include "terrain.hpp"
#include "provinces.hpp"

namespace Map
{
  enum Mode {
    TERRAIN,
    POLITICAL,
  };

  inline Mode mode = Mode::TERRAIN;

  void Draw(State&, entt::registry &, TextureCache &);

};

