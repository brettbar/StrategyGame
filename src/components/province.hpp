#pragma once

#include "../common.hpp"
#include "tile.hpp"

namespace Province {

struct Component {
  u32 id;
  i32 owner;
  bool selected = false;

  std::shared_ptr<Tile::Component> tile;
};

}// namespace Province
