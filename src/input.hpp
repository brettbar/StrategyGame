//
// Created by brett on 1/13/2022.
//

#pragma once

#include "common.hpp"
#include "resource.hpp"
#include "state.hpp"
#include "systems/movement.hpp"
#include "systems/map/provinces.hpp"
#include "systems/map/map.hpp"
#include "systems/spawn.hpp"
#include "systems/selection.hpp"

namespace Input
{
  void Handle(State &, entt::registry &, TextureCache &);
};
