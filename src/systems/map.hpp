#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "terrain.hpp"
#include <array>
#include <thread>

namespace Map
{

  struct Province {
    i32 owner;
    u32 population;
    str name;
  };

  void UpdateProvinces(entt::registry &);
  void SetProvinceOwner(entt::registry &, u32, Vector2);

  void DrawProvinces(entt::registry &, TextureCache &);
//  void DrawSingleBorder(Province());

//  Tile *FindTileByCoord(TileMap &, u32, u32);

};// namespace Map
