#pragma once
#include <map>
#include <string>

namespace Faction {

enum class ID {
  Romans,
  Greeks,
  Celts,
  Punics,
  Persians,
  Germans,
  Scythians,
};

inline std::map<ID, std::string> faction_map = {
  { ID::Romans, "Roman Republic" },
  { ID::Greeks, "Greek Cities" },
  { ID::Celts, "Celtic Tribes" },
  { ID::Punics, "Punic Colonies" },
  { ID::Persians, "Persian Empire" },
  { ID::Germans, "Germanic Federation" },
  { ID::Scythians, "Scythian Horde" },
};

};// namespace Faction
