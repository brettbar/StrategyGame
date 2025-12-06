#pragma once
#include "../../shared/common.hpp"

#include "../../data/buildings.hpp"
#include "../../data/resources.hpp"
#include "../../data/settlement.hpp"

#include "regiment_component.hpp"

#include <cereal/types/common.hpp>
#include <cereal/types/vector.hpp>

namespace Settlement {
  struct Component {
    u32 id;
    std::string name;
    Development development;
    Population population;
    Texture2D texture;
    map<Resources::Type, u32> resource_quantities = {};

    u32 max_building_slots = 1;

    list<Buildings::Building> buildings = {};

    list<Regiment> garrison = {};

    // @TODO might need separate save/load functions for this one
    template<class Archive>
    void serialize(Archive &ar) {
      ar(id, name, development, population, buildings);
    }

    // @TODO is this needed?
    template<class Archive>
    void serialize(Archive &ar, Population &pop) {
      ar(pop);
    }
  };


};// namespace Settlement
