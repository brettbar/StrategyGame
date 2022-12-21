#pragma once

#include "common.hpp"

namespace Events {
struct UIEvent {
  std::string msg;
};
struct EventEmitter : entt::emitter<EventEmitter> {};
inline EventEmitter event_emitter;


};// namespace Events
