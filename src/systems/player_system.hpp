//
// Created by brett on 12/30/2021.
//

#pragma once

#include "../global.hpp"

enum class Faction {
  ROMANS,
  GREEKS,
  CELTS,
  PUNICS,
  PERSIANS,
  GERMANS,
  SCYTHIANS,
};

namespace PlayerSystem {

inline void Init() {}

};// namespace PlayerSystem

class TempPS {
  public:
  u32 id;
  const char *factionName;
  Faction faction;
  std::map<const char *, hstr> textureMap;

  TempPS( u32 id, Faction faction, const char *factionName ) {
    this->id = id;
    this->faction = faction;
    this->factionName = factionName;
    RefreshTextureMap();
  }
  ~TempPS() {}

  void RefreshTextureMap() {
    switch ( faction ) {
      case Faction::ROMANS:
        this->textureMap = {
          {
            "Villager",
            hstr{ "romanVillagerTexture" },
          },
        };
        break;
      case Faction::GREEKS:
        this->textureMap = {
          {
            "Villager",
            hstr{ "greekVillagerTexture" },
          },
        };
        break;
      case Faction::CELTS:
        this->textureMap = {
          {
            "Villager",
            hstr{ "celtVillagerTexture" },
          },
        };
        break;
      case Faction::PUNICS:
        this->textureMap = {
          {
            "Villager",
            hstr{ "punicVillagerTexture" },
          },
        };
        break;
      case Faction::PERSIANS:
        this->textureMap = {
          {
            "Villager",
            hstr{ "persianVillagerTexture" },
          },
        };
        break;
      default:
        break;
    }
  }
};
