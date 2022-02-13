//
// Created by brett on 1/30/2022.
//

#pragma once

#include "components.hpp"


namespace Components {
  enum ActorType {
    SETTLER,
    ARMY
  };

  struct Actor {
    ActorType type;
  };
};// namespace Components
