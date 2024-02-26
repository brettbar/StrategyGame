#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

#include "../components/faction.hpp"

namespace Faction {


  // inline Texture2D villagar_texure_from_owner( entt::entity owner ) {
  //   std::string faction_id = Global::world.get<Faction::Component>( owner ).id;
  //   return Global::texture_cache[hstr{ ( faction_id + "_villager" ).c_str() }]
  //     ->texture;
  // }

  // inline Texture2D hastati_texure_from_owner( entt::entity owner ) {
  //   std::string faction_id = Global::world.get<Faction::Component>( owner ).id;
  //   return Global::texture_cache[hstr{ ( faction_id + "_hastati" ).c_str() }]
  //     ->texture;
  // }
};// namespace Faction
