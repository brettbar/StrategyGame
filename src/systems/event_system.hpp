#pragma once

#include "../common.hpp"
#include "../state.hpp"
// #include "world/systems/spawn.hpp"


namespace EventSystem {

inline entt::dispatcher dispatcher{};

struct Listener {
  State *currState;
  entt::registry *currReg;

  Listener() {
    currState = nullptr;
    currReg = nullptr;
  }
  ~Listener() {}

  Listener( State &state, entt::registry &reg ) {
    this->currState = &state;
    this->currReg = &reg;
  }

  inline void Update( State &state, entt::registry &reg ) {
    this->currState = &state, this->currReg = &reg;
  }

  inline virtual void Receive() {}
};


};// namespace EventSystem
