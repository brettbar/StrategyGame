//
// Created by brett on 12/30/2021.
//

#pragma once

#include "../common.hpp"

using u32 = unsigned int;
using i32 = int;
using f32 = float;
// using str = std::string;
using hstr = entt::hashed_string;

enum class Faction {
  ROMANS,
  GREEKS,
  CELTS,
  PUNICS,
  PERSIANS,
  GERMANS,
  SCYTHIANS,
};

class PlayerSystem {
  public:
  u32 id;
  const char *factionName;
  Faction faction;
  std::map<const char *, hstr> textureMap;

  PlayerSystem( u32 id, Faction faction, const char *factionName ) {
    this->id = id;
    this->faction = faction;
    this->factionName = factionName;
    RefreshTextureMap();
  }
  ~PlayerSystem() {}

  void RefreshTextureMap() {
    switch ( faction ) {
      case Faction::ROMANS:
        this->textureMap = { { "Villager", hstr{ "romanVillagerTexture" } } };
        break;
      case Faction::GREEKS:
        this->textureMap = { { "Villager", hstr{ "greekVillagerTexture" } } };
        break;
      case Faction::CELTS:
        this->textureMap = { { "Villager", hstr{ "celtVillagerTexture" } } };
        break;
      case Faction::PUNICS:
        this->textureMap = { { "Villager", hstr{ "punicVillagerTexture" } } };
        break;
      case Faction::PERSIANS:
        this->textureMap = { { "Villager", hstr{ "persianVillagerTexture" } } };
        break;
      default:
        break;
    }
  }
};
