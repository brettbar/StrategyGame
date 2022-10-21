//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../common.hpp"

namespace Actor {

enum class Type { Colonist, Army };

struct Component {
  const char *name;
  Type type;
};

};// namespace Actor
