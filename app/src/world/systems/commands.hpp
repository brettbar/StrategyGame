#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

namespace Commands {

  enum class Command_t {
    TimeChange,
    Spawn,
    Move,
  };

  struct Command {
    Command_t type;
    entt::entity player_e;

    str msg;
    Vector2 click_pos;
    entt::entity entity;
  };


};// namespace Commands