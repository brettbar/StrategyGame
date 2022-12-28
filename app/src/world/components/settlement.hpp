#pragma once
#include "../../shared/common.hpp"

#include "../../../data/buildings.hpp"
#include "../../../data/settlement.hpp"

#include <cereal/types/common.hpp>

namespace Settlement {

inline std::map<std::string, Image> building_map;


struct Component {
  u32 id;
  std::string name;
  Development development;
  Population population;
  Texture2D texture;

  // Building
  std::vector<Buildings::Building> buildings = {};


  // TOOD might need separate save/load functions for this one
  template<class Archive>
  void serialize( Archive &ar ) {
    ar( id, name, development, population );
  }

  template<class Archive>
  void serialize( Archive &ar, Population &pop ) {
    ar( pop );
  }

  // template<class Archive>
  // void serialize( Archive &ar, Texture2D &tex ) {
  //   ar( tex.id, tex.width, tex.height, tex.mipmaps, tex.format );
  // }
};


};// namespace Settlement
