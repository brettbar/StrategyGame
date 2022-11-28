#pragma once
#include "../common.hpp"
#include "../data/buildings.hpp"

namespace Settlement {

enum class Development {
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

inline std::map<Development, std::string> development = {
  { Development::Uninhabited, "Uninhabited" },
  { Development::Village, "Village" },
  { Development::Town, "Town" },
  { Development::City, "City" },
  { Development::Metropolis, "Metropolis" },
  { Development::Fort, "Fort" },
  { Development::Fortress, "Fortress" },
  { Development::Citadel, "Citadel" },
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

struct Component {
  u32 id;
  std::string name;
  Development development;
  Population population;
  Texture2D texture;
};


};// namespace Settlement
