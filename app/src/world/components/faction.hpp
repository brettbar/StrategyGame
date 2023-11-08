#pragma once

#include "../../shared/common.hpp"

namespace Faction
{
  struct Component
  {
    std::string id;
    std::string primary_color;
    std::string secondary_color;
    std::string adjectival;
    std::string denonym;

    template<class Archive>
    void serialize( Archive &ar )
    {
      ar( id, primary_color, secondary_color, adjectival, denonym );
    }
  };


};// namespace Faction
