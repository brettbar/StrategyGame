//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../../common.hpp"

namespace Actor {
enum ActorType { SETTLER, ARMY };

struct Component {
  ActorType type;
};
};// namespace c_Actor
