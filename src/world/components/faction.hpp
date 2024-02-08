#pragma once

#include "../../shared/common.hpp"

namespace Faction {
  struct Component {
    str id;
    str primary_color;
    str secondary_color;
    str adjectival;
    str denonym;
    list<str> settlement_names;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar(
        id,
        primary_color,
        secondary_color,
        adjectival,
        denonym,
        settlement_names
      );
    }
  };


};// namespace Faction
