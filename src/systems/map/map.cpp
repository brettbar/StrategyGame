//
// Created by brett on 2/3/2022.
//

#include "map.hpp"

namespace Map
{
  void Draw(State& state, entt::registry &reg, TextureCache& cache)
  {
    switch (mode)
    {
      case Mode::TERRAIN:
        Terrain::Draw(state.camera, reg, cache);
        Provinces::DrawProvinces(reg, cache, false);
        break;
      case Mode::POLITICAL:
        Terrain::Draw(state.camera, reg, cache);
        Provinces::DrawProvinces(reg, cache, true);
        break;
    }
  }

};