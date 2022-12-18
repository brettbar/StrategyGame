#pragma once
#include <map>
#include <string>

#include "settlement.hpp"

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

enum class Settledness {
  Settled,
  Tribal,
  Nomadic,
};

enum class Government {
  Empire,
  Republic,
  Federation,
};


inline std::map<ID, std::string> faction_names = {
  { ID::Romans, "Roman Republic" },
  { ID::Greeks, "Greek Cities" },
  { ID::Celts, "Celtic Tribes" },
  { ID::Punics, "Punic Colonies" },
  { ID::Persians, "Persian Empire" },
  { ID::Germans, "Germanic Federation" },
  { ID::Scythians, "Scythian Horde" },
};

inline std::map<ID, std::string> dev_levels = {};

struct Instance {
  ID id;
  std::string name = faction_names[id];
};

};// namespace Faction
