#pragma once

#include "common.hpp"
#include "state.hpp"
// #include "world/systems/spawn.hpp"


namespace Events
{

// enum EventType
// {
//   PROVINCES_SPAWN_PROVINCE,
//   SPAWN_DELETE_SPAWNED,
// };

inline entt::dispatcher dispatcher{};

struct Event {
};
struct ProvEvent : Event {
};
struct SpawnEvent : Event {
};


struct Listener {
  State *currState;
  entt::registry *currReg;

  Listener()
  {
    currState = nullptr;
    currReg = nullptr;
  }
  ~Listener() {}

  Listener( State &state, entt::registry &reg )
  {
    this->currState = &state;
    this->currReg = &reg;
  }

  inline void Update( State &state, entt::registry &reg )
  {
    this->currState = &state, this->currReg = &reg;
  }

  inline virtual void Receive() {}
};


};// namespace Events
