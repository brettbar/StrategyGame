#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
#include <sstream>

#include "../include/entt/entt.hpp"
#include "components/actor.hpp"
#include "components/animated.hpp"
#include "components/province.hpp"
#include "components/settlement.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"
#include "global.hpp"

// using namespace entt::literals;

namespace SaveSystem {

inline void Save( entt::registry &source ) {
  printf( "Saving to output.json\n" );

  std::ofstream file( "output.json" );
  {
    // cereal::JSONOutputArchive output{ storage };
    cereal::JSONOutputArchive output{ file };

    entt::snapshot{ source }
      .entities( output )
      .component<
        Actor::Component,
        Unit::Component,
        Animated::Component,
        Sight::Component>( output );

    printf( "%u\n", (int) source.size() );
  }
  file.close();
}

inline void Load() {
  printf( "Loading from output.json\n" );

  Global::game_started = false;
  Global::world.clear();

  std::ifstream file( "output.json" );
  {
    cereal::JSONInputArchive input{ file };

    entt::snapshot_loader{ Global::world }
      .entities( input )
      .component<
        Actor::Component,
        Unit::Component,
        Animated::Component,
        Sight::Component>( input );

    printf( "%u\n", (int) Global::world.size() );
  }
  file.close();
}

};// namespace SaveSystem
