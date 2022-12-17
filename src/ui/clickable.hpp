#pragma once

#include "../global.hpp"
#include "../systems/actor_system.hpp"


namespace UI {

inline std::map<std::string, std::function<bool()>> clickable_lookup = {
  {
    "actor_spawn_settlement_button",
    []() -> bool { return ActorSystem::ColonistCanPlaceSettlement(); },
  },
};


};
