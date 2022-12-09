#pragma once

#include "../../data/factions.hpp"
#include "../common.hpp"

namespace Player {

struct Component {
  entt::entity id;
  bool is_human;
  Faction::ID faction;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( id, is_human, faction );
  }
};

};// namespace Player
