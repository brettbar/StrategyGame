#pragma once

#include "common.hpp"
#include "state.hpp"


namespace Events
{

enum EventType
{
  PROVINCES_SPAWN_PROVINCE,
  SPAWN_DELETE_SPAWNED,
};

struct Event {
  EventType type;
};

// struct ProvincesEvent : Event {
// };

// struct SpawnEvent : Event {
// };

inline entt::dispatcher dispatcher{};

};// namespace Events
