#pragma once

#include "../../../data/resources.hpp"
#include "../../shared/common.hpp"
#include "cereal/types/vector.hpp"
#include "tile.hpp"

namespace Province {

  struct Component {
    bool selected = false;

    std::shared_ptr<Tile::Component> tile;
    std::vector<Resources::Natural> resources;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( selected, tile, CEREAL_NVP( resources ) );
    }
  };

}// namespace Province
