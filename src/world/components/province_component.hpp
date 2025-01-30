#pragma once

#include "../../data/resources.hpp"
#include "../../shared/common.hpp"
#include "tile_component.hpp"

#include <cereal/types/vector.hpp>

namespace Province {

  struct Component {
    bool selected = false;

    sptr<Tile::Component> tile;
    list<Resources::Natural> natural_resources;
    map<Resources::Type, u32> resources;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( selected, CEREAL_NVP( tile ), CEREAL_NVP( natural_resources ) ),
        CEREAL_NVP( resources );
    }
  };


}// namespace Province
