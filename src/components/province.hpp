#pragma once

#include "../common.hpp"
#include "../data/resources.hpp"
#include "cereal/types/vector.hpp"
#include "tile.hpp"

namespace Province {

struct Component {
  u32 id;// TODO should I replace this or remove with entt::entity
  entt::entity owner;
  bool selected = false;

  std::shared_ptr<Tile::Component> tile;
  std::vector<NaturalResource> resources;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( id, owner, selected, tile, CEREAL_NVP( resources ) );
  }
};

}// namespace Province
