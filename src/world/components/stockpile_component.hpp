#pragma once

#include "../../../data/resources.hpp"
#include "../../shared/common.hpp"
#include <cereal/types/common.hpp>

namespace Stockpile {
  struct Component {
    map<Resources::Type, u32> resources;

    Component() {
      resources = {};

      for ( u32 i = 0; i < (u32) Resources::Type::COUNT; i++ ) {
        Resources::Type rt = (Resources::Type) i;

        resources[rt] = 0;
      }
    }

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( CEREAL_NVP( resources ) );
    }
  };
}// namespace Stockpile
