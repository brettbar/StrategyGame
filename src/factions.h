
#pragma once

#ifndef MAP_H
#define MAP_H

#include "../include/entt.hpp"
#include "raylib.h"

using u32 = unsigned int;
using i32 = int;
using f32 = float;

namespace Factions {

#define MAX_PLAYERS 5

enum Civilization {
  //forsure
  Roman_Republic,
  Greek_Cities,
  Celtic_Tribes,
  Persian_Empire,
  Punic_Colonies,
  //unsure
  Germanic_Federation,
  Egyptian_Kingdom,
  Macedonian_Kingdoms,
  Gupta_Empire,
  Scythian_Horde,
  //really unsure
  Han_Dynasty,
  Iberian_Tribes,
  Dacian_Kingdom,
  Thracian_Tribes,
};

struct Faction {
  u32 playerId;
  Civilization civ;
  Color primary;
  Color secondary;
};

void InitPlayers(entt::registry &, u32);
}; // namespace Faction

#endif
