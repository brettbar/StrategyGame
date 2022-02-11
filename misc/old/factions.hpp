
#pragma once

#ifndef FACTIONS_H
#define FACTIONS_H

// #include "../include/entt.hpp"
#include "raylib.h"
#include <string>
#include <vector>

using u32 = unsigned int;
using i32 = int;
using f32 = float;

namespace Factions {

// #define MAX_PLAYERS 6

enum Government {
  /*
    One despotic Ruler and his council run the government.
  */
  Monarchy,
  /*
    A senate of aristocrats run the government.
  */
  Republic,
  /*
    A coalition of tribes/cities hold stake on how to run the government.
    Done via a war council or council of elders.
  */
  Federation,
};

enum Mobility {
  /*
    Faction can and frequently moves provinces.
    Settlements can pickup and become a unit, moving like an army would.

    Has no formal Development level, instead using population size and wealth?
    as metric.

    Settlement buildings are built in the form of urts/tents, or temporary
    buildings that do not persist when moving.
  */
  Nomadic,
  /*
    Faction settlements are established, but can slowly move tile-by-tile.
    Population is quite spread out, prefers to sprawl out with medium-sized
    settlements.
    (Many medium-dev settlements)

    Lacks access to Metropolis and Citadel development levels.

    Builds buildings in settlements.
  */
  Tribal,
  /*
    Factions settlements are static, and cannot be moved without abandoning
    the settlement.
    Population is consentrated in large-population centers.
    (Some high-dev settlements, many low-dev settlements).

    Has access to all development levels.

    Builds buildings in settlements.
  */
  Settled,
};

// Settlement Development Level
enum Development {
  Village,

  Town,
  City,
  Metropolis,

  Fort,
  Fortress,
  Citadel
};

struct Society {
  Mobility mobility;
  std::string culture;  
  std::string religion;
  std::vector<std::string> commonNames;
  std::vector<std::string> settlementNames;
};

struct CivTrait {
  std::string name;
  std::string description;
  // status for actual bonus
};

struct Civilization {
  std::string name;
  std::string adjective;
  std::string description;
  Color primary;
  Color secondary;
  Government government;
  Society society;
  std::vector<CivTrait> uniqueTraits;
  std::vector<CivTrait> uniqueMechanics;
};

struct Faction {
  u32 playerId;
  Civilization civ;
};


}; // namespace Factions

#endif
