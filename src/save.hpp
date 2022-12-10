#pragma once

#include "../include/entt/entt.hpp"
#include "components/actor.hpp"
#include "components/animated.hpp"
#include "components/player.hpp"
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

    // entt::snapshot{ Global::world }
    //   .entities( output )
    //   .component<
    //     Actor::Component,
    //     Unit::Component,
    //     Animated::Component,
    //     Sight::Component>( output );


    // Think the problem with player is that since we are
    // serializing an entity it keeps it alive or something
    // and has issues with PlayerSystem::Init


    entt::snapshot{ Global::world }
      .entities( output )
      .component<Player::Component>( output );

    // entt::snapshot{ Global::world }
    //   .entities( output )
    //   .component<Province::Component>( output );

    // entt::snapshot{ Global::world }
    //   .entities( output )
    //   .component<Province::Component>( output );

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

    // entt::snapshot_loader{ Global::world }
    //   .entities( input )
    //   .component<
    //     Actor::Component,
    //     Unit::Component,
    //     Animated::Component,
    //     Sight::Component>( input );


    entt::snapshot_loader{ Global::world }
      .entities( input )
      .component<Player::Component>( input );

    // entt::snapshot_loader{ Global::world }
    //   .entities( input )
    //   .component<Province::Component>( input );

    // entt::snapshot_loader{ Global::world }
    //   .entities( input )
    //   .component<Province::Component>( input );


    // printf( "%u\n", (int) Global::world.size() );
  }
  file.close();
}

};// namespace SaveSystem
