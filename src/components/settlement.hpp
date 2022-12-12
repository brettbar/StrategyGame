#pragma once
#include "../common.hpp"
#include "../data/buildings.hpp"
#include <cereal/types/common.hpp>

namespace Settlement {

inline std::map<std::string, Image> building_map;

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

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( current, birthRate, deathRate, growthRate, carryingCapacity );
  }
};

struct Component {
  u32 id;
  std::string name;
  Development development;
  Population population;
  Texture texture;

  // TOOD might need separate save/load functions for this one
  template<class Archive>
  void serialize( Archive &ar ) {
    ar( id, name, development, population, texture );
  }

  template<class Archive>
  void serialize( Archive &ar, Population &pop ) {
    ar( pop );
  }

  template<class Archive>
  void serialize( Archive &ar, Texture &tex ) {
    ar( tex.id, tex.width, tex.height, tex.mipmaps, tex.format );
  }
};


};// namespace Settlement
