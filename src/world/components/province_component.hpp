#pragma once

#include "../../data/biomes.hpp"
#include "../../data/resources.hpp"
#include "../../shared/common.hpp"

#include <cereal/types/vector.hpp>

namespace Province {

  enum class Visibility {
    UNEXPLORED,
    EXPLORED,
    VISIBILE,
  };

  struct Tile {
    entt::entity prov_entity = entt::null;

    u32 id;
    f32 noise;
    Vector2 position;
    Vector2 center;
    vec2u coords;
    Biome biome;
    Visibility visibility;
    // str texture_key;
    u32 texture_i;

    template<class Archive>
    void serialize(Archive &ar) {
      ar(
        prov_entity,
        cereal::make_nvp("tile_id", id),
        noise,
        position.x,
        position.y,
        center.x,
        center.y,
        coords.x,
        coords.y,
        biome,
        visibility,
        texture_i
      );
    }
  };


  struct Component {
    bool selected = false;// @todo do we even need this anymore
                          //
    // I don't like this
    entt::entity owner = entt::null;

    Tile tile;
    list<Resources::Natural> natural_resources;
    map<Resources::Type, u32> resources;

    template<class Archive>
    void serialize(Archive &ar) {
      ar(selected, owner, tile, natural_resources, resources);
    }
  };


}// namespace Province
