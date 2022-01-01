#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "terrain.hpp"
#include <array>
#include <thread>

namespace Map
{
  struct Province {
    u32 id;
    i32 owner;
    u32 population;
    str name;
    std::shared_ptr<Terrain::Tile> tile;
  };

  using Provinces =
    std::array<std::shared_ptr<Province>, Terrain::MAP_WIDTH*Terrain::MAP_HEIGHT>;

  void InitProvinces(entt::registry &);
  void UpdateProvinces(entt::registry &);
  void SetProvinceOwner(entt::registry &, u32, Vector2);

  void DrawProvinces(entt::registry &, TextureCache &);
//  void DrawSingleBorder(Province());


};// namespace Map
