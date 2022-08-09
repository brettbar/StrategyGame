#pragma once

#include "../common.hpp"
#include "tile.hpp"

namespace Province {

enum Development {
  Uninhabited,
  Village,
  // Civic
  Town,
  City,
  Metropolis,
  // Martial
  Fort,
  Fortress,
  Citadel,
};

struct Population {
  u32 current;

  // births per 1000 people
  // higher when health is higher and degeneracy is lower
  f32 birthRate;
  // deaths per 1000 people
  // higher when health is lower
  f32 deathRate;

  // birthRate - deathRate
  f32 growthRate;

  // higher when disease and famine are lower
  f32 health;

  // higher when the current population is higher
  f32 foodConsumption;

  // higher when food foodConsumption is lower and external factors
  f32 foodAvailability;

  // higher when food consumption is higher and foodAvailability is lower
  f32 famine;

  // higher when density is higher, degeneracy is higher, and external factors
  f32 disease;

  // higher by external factors
  f32 degeneracy;

  // d = current / carryingCapacity
  f32 density;

  // higher when settlment development is higher
  f32 carryingCapacity;
};

// TODO move to its own file
struct Settlement {
  u32 id;
  const char *name;
  Development development;
  Population population;
  Texture texture;
};

struct Component {
  u32 id;
  i32 owner;

  Settlement settlement;
  std::shared_ptr<Tile::Component> tile;
};

}// namespace Province
