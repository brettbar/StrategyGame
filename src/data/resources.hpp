// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

#include "../../src/shared/common.hpp"
#include <string>
#include <vector>

namespace Resources {
  enum class Natural {
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

  inline std::string GetRawMaterialName( RawMaterial mat ) {
    switch ( mat ) {
      case RawMaterial::Wheat:
        return "wheat";
      default:
        return "INVALID";
    }
  }

  // enum class ProcessedMaterial {
  //   Lumber,
  //   Flour,
  //   Bricks,
  //   StoneBricks,
  //   Pottery,
  //   Cloth,
  //   Linen,
  //   Leather,
  //   Silver,
  //   Gold,
  //   Iron,
  //   Bronze,
  // };

  // enum class Product {
  //   OliveOil,
  //   Swords,
  //   Spears,
  //   Shields,
  //   Helmets,
  //   Armour,
  //   Clothing,
  //   Jewelry,
  // };

  // inline std::vector<std::string> natural = {
  //   "soil",
  //   "trees",
  //   "fish",
  //   "clay",
  //   "stone",
  //   "sheep",
  //   "cattle",
  //   "silver_vein",
  //   "gold_vein",
  //   "iron_vein",
  //   "tin_vein",
  //   "copper_vein",
  //   "gemstones",
  // };

  inline std::vector<std::string> raw = {
    "timber",
    "wheat",
    "olives",
    "fish",
    "clay",
    "stone",
    "wool",
    "flax",
    "hide",
    "silver_ore",
    "gold_ore",
    "iron_ore",
    "tin_ore",
    "copper_ore",
    "gems",
  };

  inline std::vector<std::string> refined = {
    "lumber",
    "flour",
    "bricks",
    "stone_bricks",
    "pottery",
    "cloth",
    "linen",
    "leather",
    "silver",
    "gold",
    "iron",
    "bronze",
  };

  inline std::vector<std::string> products = {
    "olive_oil",
    "swords",
    "spears",
    "shields",
    "helmets",
    "armour",
    "clothing",
    "jewelry",
  };

};// namespace Resources
