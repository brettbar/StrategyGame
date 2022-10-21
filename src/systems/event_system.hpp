#pragma once

#include "../common.hpp"
#include "../state.hpp"
// #include "world/systems/spawn.hpp"


namespace EventSystem {

inline entt::dispatcher dispatcher{};

struct Listener {
  State *currState;

  Listener() { currState = nullptr; }
  ~Listener() {}

  Listener( State &state ) { this->currState = &state; }

  inline void Update( State &state ) { this->currState = &state; }

  inline virtual void Receive() {}
};


};// namespace EventSystem
