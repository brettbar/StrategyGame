/*
  This is where all UI actions will go. I centralize them here because that is easier to keep track of 
  than having the actions distributed to each ui elements file, importing stuff randomly all over.
  
  NOTE: When adding a new ui action, add both an action and clickable lambda to each of these looks up
*/

#pragma once

#include "../common.hpp"
#include "../systems/actor_system.hpp"
#include "../systems/settlement_system.hpp"

namespace UI {

inline std::map<std::string, std::function<void()>> action_lookup = {
  {
    "actor_spawn_settlement_button",
    []() {
      printf( "Spawn Settlement clicked!!\n" );
      SettlementSystem::SpawnSettlement();
    },
  },
  {
    "context_tab_button",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
};

inline std::map<std::string, std::function<bool()>> clickable_lookup = {
  {
    "actor_spawn_settlement_button",
    []() -> bool { return ActorSystem::ColonistCanPlaceSettlement(); },
  },
  {
    "context_tab_button",
    []() -> bool { return true; },
  },
};

};// namespace UI
