// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

#include <string>
#include <vector>

#include "biomes.hpp"
#include "buildings.hpp"

enum class NaturalResource {
  Soil,// not sure about this one
  Trees,
  Fish,
  Clay,
  Stone,
  Sheep,
  Cattle,
  SilverVein,
  GoldVein,
  IronVein,
  TinVein,
  CopperVein,
  Gemstones,
};

enum class RawMaterial {
  Timber,
  Wheat,
  Olives,
  Fish,
  Clay,
  Stone,
  Wool,
  Flax,
  Hide,
  SilverOre,
  GoldOre,
  IronOre,
  TinOre,
  CopperOre,
  Gems,
};

enum class ProcessedMaterial {
  Lumber,
  Flour,
  Bricks,
  StoneBricks,
  Pottery,
  Cloth,
  Linen,
  Leather,
  Silver,
  Gold,
  Iron,
  Bronze,
};

enum class Product {
  OliveOil,
  Swords,
  Spears,
  Shields,
  Helmets,
  Armour,
  Clothing,
  Jewelry,
};

struct RawMaterialRecipe {
  NaturalResource input;
  GatheringBuilding building;
  std::vector<Biome> biome;
};

struct ProcessedMaterialRecipe {
  std::vector<RawMaterial> inputs;
  ProcessingBuilding building;
};

struct ProductRecipe {
  std::vector<RawMaterial> raw_inputs;
  std::vector<ProcessedMaterial> processed_inputs;
  ProductionBuilding building;
};
