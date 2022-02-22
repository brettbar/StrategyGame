#pragma once

#include "common.hpp"
#include "state.hpp"


namespace Events
{

enum EventType
{
  PROVINCES_SPAWN_PROVINCE,
};

struct Event {
  EventType type;
};

struct EventEmitter : entt::emitter<EventEmitter> {
};

EventEmitter emitter{};

};// namespace Events
