#pragma once

#include "../../data/resources.hpp"
#include "../../shared/common.hpp"
#include "cereal/types/vector.hpp"
#include "tile_component.hpp"

namespace Province {

  struct Component {
    bool selected = false;

    sptr<Tile::Component> tile;
    list<Resources::Natural> resources;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( selected, tile, CEREAL_NVP( resources ) );
    }
  };

}// namespace Province
