// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

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

  enum class Type {
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
    //////////////////
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
    //////////////////
    //   OliveOil,
    //   Swords,
    //   Spears,
    //   Shields,
    //   Helmets,
    //   Armour,
    //   Clothing,
    //   Jewelry,
    COUNT,
  };


  inline const char *ResourceStr( Type r ) {
    switch ( r ) {
      case Type::Timber:
        return "timber";
      case Type::Wheat:
        return "wheat";
      // case Type::Olives:
      //   return "olives";
      case Type::Fish:
        return "fish";
      case Type::Clay:
        return "clay";
      // case Type::Stone:
      //   return "stone";
      // case Type::Wool:
      //   return "wool";
      // case Type::Flax:
      //   return "flax";
      // case Type::Hide:
      //   return "hide";
      case Type::SilverOre:
        return "silver ore";
      case Type::GoldOre:
        return "gold ore";
      case Type::IronOre:
        return "iron ore";
      case Type::TinOre:
        return "tin ore";
      case Type::CopperOre:
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

  inline std::vector<const char *> resources = {
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
    //////////////////
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
    ///////////////////
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
