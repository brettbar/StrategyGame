#pragma once

#include "../include/entt/entt.hpp"
#include "components/actor.hpp"
#include "components/animated.hpp"
#include "components/province.hpp"
#include "components/settlement.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"
#include "global.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
// #include <sstream>

// using namespace entt::literals;

namespace SaveSystem {

inline void Save() {
  printf( "Saving to output.json\n" );

  std::ofstream file( "output.json" );
  {
    cereal::JSONOutputArchive output{ file };

    /*
     * For some reason when this component<>thing 
     * has more than 1 type given to it we get strange linker 
     * errors
     */
    entt::snapshot{ Global::world }
      .entities( output )
      .component<Unit::Component>( output );

    // printf( "%u\n", (int) source.size() );
  }
  file.close();
}

inline void Load() {
  printf( "Loading from output.json\n" );


  std::ifstream file( "output.json" );
  {
    cereal::JSONInputArchive input{ file };

    Global::ClearRegistry();
    assert( Global::world.empty() );

    /*
     * For some reason when this component<>thing 
     * has more than 1 type given to it we get strange linker 
     * errors
     */
    entt::snapshot_loader{ Global::world }
      .entities( input )
      .component<Unit::Component>( input );

    // printf( "%u\n", (int) Global::world.size() );
  }
  file.close();
}

};// namespace SaveSystem
