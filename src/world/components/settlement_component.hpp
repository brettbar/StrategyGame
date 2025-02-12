#pragma once
#include "../../shared/common.hpp"

#include "../../data/buildings.hpp"
#include "../../data/resources.hpp"
#include "../../data/settlement.hpp"

#include "regiment_component.hpp"

#include <cereal/types/common.hpp>
#include <cereal/types/vector.hpp>

namespace Settlement {

  struct Building {
    Buildings::BuildingType type;
    u32 current_recipe = 0;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( type, current_recipe );
    }
  };


  struct Component {
    u32 id;
    std::string name;
    Development development;
    Population population;
    Texture2D texture;
    std::map<Resources::Type, u32> resources = {};

    // Building
    std::vector<Building> buildings = {};

    std::vector<Regiment> garrison = {};

    // TOOD might need separate save/load functions for this one
    template<class Archive>
    void serialize( Archive &ar ) {
      ar( id, name, development, population, buildings );
    }

    template<class Archive>
    void serialize( Archive &ar, Population &pop ) {
      ar( pop );
    }
  };


};// namespace Settlement
