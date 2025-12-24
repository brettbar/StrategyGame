// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

#include "../src/shared/common.hpp"
#include "../src/shared/utils.hpp"

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
  Olives,
  Fish,
  Clay,
  // Wool,
  // Flax,
  // Hide,
  Bricks,
  Stone,
  SilverOre,
  GoldOre,
  IronOre,
  TinOre,
  CopperOre,
  // Gems,
  //////////////////
  // Flour,
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
  ShortSwords,
  LongSwords,
  Spears,
  SmallShields,
  Helmets,
  //   Armour,
  //   Clothing,
  //   Jewelry,
  COUNT,
};


inline const char *resource_cstr(Type r) {
  switch (r) {
    case Type::Timber:
      return "Timber";
    case Type::Wheat:
      return "Wheat";
    // case Type::Olives:
    //   return "olives";
    case Type::Fish:
      return "Fish";
    case Type::Clay:
      return "Clay";
    // case Type::Stone:
    //   return "stone";
    // case Type::Wool:
    //   return "wool";
    // case Type::Flax:
    //   return "flax";
    // case Type::Hide:
    //   return "hide";
    case Type::Bricks:
      return "Bricks";
    case Type::Stone:
      return "Stone";
    case Type::SilverOre:
      return "Silver Ore";
    case Type::GoldOre:
      return "Gold Ore";
    case Type::IronOre:
      return "Iron Ore";
    case Type::TinOre:
      return "Tin Ore";
    case Type::CopperOre:
      return "Copper Ore";


    case Type::GoldIngots:
      return "Gold Ingots";
    case Type::IronIngots:
      return "Iron Ingots";
    case Type::BronzeIngots:
      return "Bronze Ingots";
      // case RawMaterial::Gems:
      //   return "gems";

    case Type::COUNT:
      return "INVALID";
  }
}


inline hstr resource_icon_path(Type type) {
  return file_path_str_format(resource_cstr(type), ".png");
}

};// namespace Resources
