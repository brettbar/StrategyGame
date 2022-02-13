//
// Created by brett on 12/30/2021.
//

#pragma once

#include "../common.hpp"

using u32 = unsigned int;
using i32 = int;
using f32 = float;
using str = std::string;
using hstr = entt::hashed_string;

enum Faction {
  ROMANS,
  GREEKS,
  CELTS,
  PUNICS,
  PERSIANS,
  GERMANS,
  SCYTHIANS,
};

class Player {
  public:
  u32 id;
  str factionName;
  Faction faction;
  std::map<str, hstr> textureMap;

  Player(u32 id, Faction faction, str factionName) {
    this->id = id;
    this->faction = faction;
    this->factionName = factionName;
    RefreshTextureMap();
  }
  ~Player() {}

  void RefreshTextureMap() {
    switch (faction) {
      case ROMANS:
        this->textureMap = {
          {"Villager", hstr{"romanVillagerTexture"}}};
        break;
      case GREEKS:
        this->textureMap = {
          {"Villager", hstr{"greekVillagerTexture"}}};
        break;
      case CELTS:
        this->textureMap = {
          {"Villager", hstr{"celtVillagerTexture"}}};
        break;
      case PUNICS:
        this->textureMap = {
          {"Villager", hstr{"punicVillagerTexture"}}};
        break;
      case PERSIANS:
        this->textureMap = {
          {"Villager", hstr{"persianVillagerTexture"}}};
        break;
      default:
        break;
    }
  }
};
