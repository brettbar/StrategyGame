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
  // Raw Materials
  Clay,
  Stone,
  Marble,
  Timber,
  SilverOre,
  GoldOre,
  IronOre,
  TinOre,
  CopperOre,
  Bricks,
  Wheat,
  Barley,
  Millet,
  Olives,
  Flax,
  Grapes,
  Flour,
  Bread,
  OliveOil,
  Seabass,
  Mackerel,
  RedMullet,
  Puls,
  Pottery,
  Linen,
  GoldIngots,
  IronIngots,
  BronzeIngots,
  SilverIngots,
  ShortSwords,
  LongSwords,
  Spears,
  SmallShields,
  LargeShields,
  Helmets,
  Bows,
  Javelins,
  Linothorax,
  Slings,
  Clothing,
  COUNT,
};


inline const char *resource_cstr(Type r) {
  switch (r) {
    // Raw Materials
    case Type::Clay:
      return "Clay";
    case Type::Stone:
      return "Stone";
    case Type::Marble:
      return "Marble";
    case Type::Timber:
      return "Timber";
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
    case Type::Bricks:
      return "Bricks";

    // Food & Agriculture
    case Type::Wheat:
      return "Wheat";
    case Type::Barley:
      return "Barley";
    case Type::Millet:
      return "Millet";
    case Type::Olives:
      return "Olives";
    case Type::Flax:
      return "Flax";
    case Type::Grapes:
      return "Grapes";
    case Type::Flour:
      return "Flour";
    case Type::Bread:
      return "Bread";
    case Type::OliveOil:
      return "Olive Oil";
    case Type::Seabass:
      return "Seabass";
    case Type::Mackerel:
      return "Mackerel";
    case Type::RedMullet:
      return "Red Mullet";
    case Type::Puls:
      return "Puls";

    // Refined Goods
    case Type::Pottery:
      return "Pottery";
    case Type::Linen:
      return "Linen";
    case Type::GoldIngots:
      return "Gold Ingots";
    case Type::IronIngots:
      return "Iron Ingots";
    case Type::BronzeIngots:
      return "Bronze Ingots";
    case Type::SilverIngots:
      return "Silver Ingots";
    case Type::Clothing:
      return "Clothing";

    // Military Equipment
    case Type::ShortSwords:
      return "Short Swords";
    case Type::LongSwords:
      return "Long Swords";
    case Type::Spears:
      return "Spears";
    case Type::SmallShields:
      return "Small Shields";
    case Type::LargeShields:
      return "Large Shields";
    case Type::Helmets:
      return "Helmets";
    case Type::Bows:
      return "Bows";
    case Type::Javelins:
      return "Javelins";
    case Type::Linothorax:
      return "Linothorax";
    case Type::Slings:
      return "Slings";

    // Meta
    case Type::COUNT:
      return "INVALID";

    // Default case to handle potential uninitialized values or warnings
    default:
      return "UNKNOWN";
  }
}


inline hstr resource_icon_path(Type type) {
  return file_path_str_format(resource_cstr(type), ".png");
}

};// namespace Resources
