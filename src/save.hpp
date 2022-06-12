#pragma once

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <sstream>

#include "../include/entt/entt.hpp"
#include "components/animated.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"

using namespace entt::literals;

namespace Save {

inline void Save( entt::registry &source ) {
  printf( "Saving to output.json\n" );
  std::ofstream file( "output.json" );

  {

    // cereal::JSONOutputArchive output{ storage };
    cereal::JSONOutputArchive output{ file };

    entt::snapshot{ source }.entities( output ).component<Unit::Component, Animated::Component, Sight::Component>( output );

    printf("%u\n", (int)source.size());
  }
}

inline entt::registry Load() {
  entt::registry reg;

  printf( "Loading from output.json\n" );

  std::ifstream file( "output.json" );

  cereal::JSONInputArchive input{ file };

  entt::snapshot_loader{ reg }
    .entities( input )
    .component<Unit::Component, Animated::Component, Sight::Component>( input );

  printf("%u\n", (int)reg.size());


  return reg;
}


};// namespace Save
