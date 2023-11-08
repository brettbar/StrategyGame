#pragma once

#include "../../shared/common.hpp"


enum UnitType
{
  Hastati,
};

struct Regiment
{
  u32 id;
  UnitType unit_type;
  u32 number;
};
