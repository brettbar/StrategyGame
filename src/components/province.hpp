#pragma once

#include "../../data/resource.hpp"
#include "../common.hpp"
#include "tile.hpp"

namespace Province {

struct Component {
  u32 id;
  entt::entity owner;
  bool selected = false;

  std::shared_ptr<Tile::Component> tile;
  std::vector<Resource::RawMaterial> raw_materials;
};

}// namespace Province
