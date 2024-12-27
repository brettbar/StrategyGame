#pragma once

#include "../src/shared/common.hpp"

namespace Factions {
  const int num_factions = 7;

  // TODO read this in from a dedicated faction.hpp or something like that
  const str factions[num_factions] = {
    "romans",
    "greeks",
    "celts",
    "carthaginians",
    "persians",
    "scythians",
    "germans",
  };
  // TODO read this in from a dedicated faction.hpp or something like that
  const Color colors[num_factions] = {
    RED,
    BLUE,
    GREEN,
    PURPLE,
    ORANGE,
    PINK,
    GRAY,
  };


  inline map<str, Color> faction_color_lookup = {
    { "romans", RED },
    { "greeks", BLUE },
    { "celts", GREEN },
    { "carthaginians", PURPLE },
    { "persians", ORANGE },
    { "scythians", PINK },
    { "germans", GRAY },
  };


}// namespace Factions