#pragma once

#include "../common.hpp"
#include "../data/factions.hpp"

namespace Player {

struct Component {
  // u32 id; this will be replaced with the entt::entity
  entt::entity id;
  bool is_human;
  Faction::ID faction;
};

};// namespace Player
