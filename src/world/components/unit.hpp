#pragma once


#include "../../shared/common.hpp"

namespace Unit {
  struct Component {
    str id;
    str faction;
    u32 tier;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( id, faction, tier );
    }
  };
};// namespace Unit
