// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

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
    // Olives,
    Fish,
    Clay,
    // Stone,
    // Wool,
    // Flax,
    // Hide,
    SilverOre,
    GoldOre,
    IronOre,
    TinOre,
    CopperOre,
    // Gems,
    COUNT,
  };

  enum class ProcessedMaterial {
    // Flour,
    // Bricks,
    // StoneBricks,
    // Pottery,
    // Cloth,
    // Linen,
    // Leather,
    // Silver,
    GoldIngots,
    IronIngots,
    BronzeIngots,
  };

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

  inline const char *GetRawMaterialName( RawMaterial mat ) {
    switch ( mat ) {
      case RawMaterial::Timber:
        return "timber";
      case RawMaterial::Wheat:
        return "wheat";
      // case RawMaterial::Olives:
      //   return "olives";
      case RawMaterial::Fish:
        return "fish";
      case RawMaterial::Clay:
        return "clay";
      // case RawMaterial::Stone:
      //   return "stone";
      // case RawMaterial::Wool:
      //   return "wool";
      // case RawMaterial::Flax:
      //   return "flax";
      // case RawMaterial::Hide:
      //   return "hide";
      case RawMaterial::SilverOre:
        return "silver ore";
      case RawMaterial::GoldOre:
        return "gold ore";
      case RawMaterial::IronOre:
        return "iron ore";
      case RawMaterial::TinOre:
        return "tin ore";
      case RawMaterial::CopperOre:
        return "copper ore";
      // case RawMaterial::Gems:
      //   return "gems";
      default:
        return "INVALID";
    }
  }


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

  inline std::vector<const char *> raw = {
    "timber",
    "wheat",
    //"olives",
    "fish",
    "clay",
    //"stone",
    //"wool",
    //"flax",
    //"hide",
    "silver ore",
    "gold ore",
    "iron ore",
    "tin ore",
    "copper ore",
    //"gems",
  };

  inline std::vector<const char *> refined = {
    // "flour",
    // "bricks",
    // "stone_bricks",
    // "pottery",
    // "cloth",
    // "linen",
    // "leather",
    // "silver",
    "gold ingots",
    "iron ingots",
    "bronze ingots",
  };

  inline std::vector<const char *> products = {
    "olive oil",
    // "swords",
    // "spears",
    // "shields",
    // "helmets",
    // "armour",
    // "clothing",
    // "jewelry",
  };

};// namespace Resources
