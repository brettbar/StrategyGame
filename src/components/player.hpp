//
// Created by brett on 12/30/2021.
//

#pragma once

#include "../common.hpp"

enum Faction {
  ROMANS,
  GREEKS,
  CELTS,
  GERMANS,
  PUNICS,
  PERSIANS,
  SCYTHIANS,
};

struct Player {
  u32 id;
  Faction faction;
  // owned provinces;
};