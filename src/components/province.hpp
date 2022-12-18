#pragma once

#include "../common.hpp"
#include "../data/resources.hpp"
#include "cereal/types/vector.hpp"
#include "tile.hpp"

namespace Province {

struct Component {
  entt::entity owner;
  bool selected = false;

  std::shared_ptr<Tile::Component> tile;
  std::vector<Resources::Natural> resources;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( owner, selected, tile, CEREAL_NVP( resources ) );
  }
};

}// namespace Province
