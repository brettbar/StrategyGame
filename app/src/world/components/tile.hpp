#pragma once

#include "../../../data/biomes.hpp"
#include "../../shared/common.hpp"

namespace Tile {

  enum class Visibility {
    UNEXPLORED,
    EXPLORED,
    VISIBILE,
  };

  // @note Not part of the ECS
  struct Component {

    // I don't like this
    // its a dup of the owner in Province::Component
    entt::entity owner = entt::null;

    u32 id;
    f32 noise;
    Vector2 position;
    Vector2 center;
    vec2u coords;
    Biome biome;
    Visibility visibility;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar(
        id,
        noise,
        position.x,
        position.y,
        center.x,
        center.y,
        coords.x,
        coords.y,
        biome,
        visibility
      );
    }
  };


};// namespace Tile
