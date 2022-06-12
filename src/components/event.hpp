//
// Created by brett on 3/22/2022.
//
#pragma once

#include <string>
namespace Event {
// enum EventType
// {
//   PROVINCES_SPAWN_PROVINCE,
//   SPAWN_DELETE_SPAWNED,
// };

struct Event {};
struct SelectionEvent : Event {
  std::string name;
};
struct SpawnEvent : Event {};
struct ProvEvent : Event {};
};// namespace Event
