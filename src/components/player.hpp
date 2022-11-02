#pragma once

#include "../data/factions.hpp"

namespace Player {

struct Component {
  // u32 id; this will be replaced with the entt::entity
  bool is_human;
  Faction::ID faction;
};

};// namespace Player
