#pragma once

#include "../include/entt/entt.hpp"
#include "../world/components/actor.hpp"
#include "../world/components/animated.hpp"
#include "../world/components/player.hpp"
#include "../world/components/province.hpp"
#include "../world/components/settlement.hpp"
#include "../world/components/sight.hpp"
#include "global.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
// #include <sstream>

// using namespace entt::literals;

namespace SaveSystem
{

  inline void Save()
  {
    printf( "Saving to output\n" );

    std::ofstream file( "output.dat", std::ios::binary );
    {
      cereal::BinaryOutputArchive output{ file };

      entt::snapshot{ Global::world }
        .entities( output )
        .component<
          Player::Component,
          Province::Component,
          Settlement::Component,
          Actor::Component,
          Animated::Component,
          Sight::Component>( output );

      // printf( "%u\n", (int) source.size() );
    }
    file.close();
  }

  inline void Load()
  {
    printf( "Loading from output\n" );


    std::ifstream file( "output.dat", std::ios::binary );
    {
      cereal::BinaryInputArchive input{ file };

      Global::ClearRegistry();

      entt::snapshot_loader{ Global::world }
        .entities( input )
        .component<
          Player::Component,
          Province::Component,
          Settlement::Component,
          Actor::Component,
          Animated::Component,
          Sight::Component>( input );

      // printf( "%u\n", (int) Global::world.size() );
    }
    file.close();
  }

};// namespace SaveSystem
