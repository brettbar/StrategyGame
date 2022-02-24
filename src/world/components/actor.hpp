//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../../common.hpp"

namespace c_Actor
{
enum ActorType
{
  SETTLER,
  ARMY
};

struct Actor {
  ActorType type;
};
};// namespace c_Actor
