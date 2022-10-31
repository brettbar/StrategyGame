#pragma once

#include "../common.hpp"
#include "../data/resource.hpp"
#include "tile.hpp"

namespace Province {

struct Component {
  u32 id;
  i32 owner;
  bool selected = false;

  std::shared_ptr<Tile::Component> tile;
  std::vector<Resource::RawMaterial> raw_materials;
};

}// namespace Province
