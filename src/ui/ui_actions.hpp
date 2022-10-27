#pragma once

#include "../common.hpp"
#include "../systems/actor_system.hpp"
#include "../systems/settlement_system.hpp"
#include "ui_components.hpp"

namespace UI {

inline std::map<entt::entity, std::function<void()>> action_lookup = {
  {
    lookup.at( "actor_spawn_settlement_button" ),
    []() {
      printf( "Spawn Settlement clicked!!\n" );
      SettlementSystem::SpawnSettlement();
    },
  },
};

inline std::map<entt::entity, std::function<bool()>> clickable_lookup = { {
  lookup.at( "actor_spawn_settlement_button" ),
  []() -> bool { return ActorSystem::ColonistCanPlaceSettlement(); },
} };

};// namespace UI
