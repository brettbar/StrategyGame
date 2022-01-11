#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "terrain.hpp"
#include <array>
#include <thread>

namespace Provinces
{
  enum Development
  {
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

  struct Settlement {
    u32 id;
    str name;
    Development development;
    Population population;
  };

  struct Province {
    u32 id;
    i32 owner;

    std::unique_ptr<Settlement> settlement;
    std::shared_ptr<Terrain::Tile> tile;
  };

  using ProvinceList = std::array<std::shared_ptr<Province>,
                                  Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT>;

  void InitProvinces(entt::registry &);
  void UpdateProvinces(entt::registry &);

  void UpdateSettlement(Settlement &);
  void UpdatePopulation(Settlement &);
  void SetProvinceOwner(entt::registry &, u32, Vector2);

  void DrawProvinces(Player, entt::registry &, TextureCache &);
  //  void DrawSingleBorder(Province());


};// namespace Provinces
