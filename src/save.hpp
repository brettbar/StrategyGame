#pragma once

#include "../include/entt/entt.hpp"
#include "components/actor.hpp"
#include "components/animated.hpp"
#include "components/province.hpp"
#include "components/settlement.hpp"
#include "components/sight.hpp"
#include "components/unit.hpp"
#include "global.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
// #include <sstream>

// using namespace entt::literals;

namespace SaveSystem {

inline void Save() {
  printf( "Saving to output\n" );

  std::ofstream file( "output.dat", std::ios::binary );
  {
    cereal::BinaryOutputArchive output{ file };

    /*
     * Animated::Component will not work here with the others for some reason
     */
    entt::snapshot{ Global::world }
      .entities( output )
      .component<
        Actor::Component,
        Unit::Component,
        Animated::Component,
        Sight::Component>( output );

    // entt::snapshot{ Global::world }
    //   .entities( output )
    //   .component<Province::Component, Settlement::Component>( output );

    // printf( "%u\n", (int) source.size() );
  }
  file.close();
}

inline void Load() {
  printf( "Loading from output\n" );


  std::ifstream file( "output.dat", std::ios::binary );
  {
    cereal::BinaryInputArchive input{ file };

    Global::ClearRegistry();
    assert( Global::world.empty() );

    /*
     * Animated::Component will not work here with the others for some reason
     */
    entt::snapshot_loader{ Global::world }
      .entities( input )
      .component<
        Actor::Component,
        Unit::Component,
        Animated::Component,
        Sight::Component>( input );

    // entt::snapshot_loader{ Global::world }
    //   .entities( input )
    //   .component<Province::Component, Settlement::Component>( input );


    // printf( "%u\n", (int) Global::world.size() );
  }
  file.close();
}

};// namespace SaveSystem
