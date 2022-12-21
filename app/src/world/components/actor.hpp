//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../common.hpp"

namespace Actor {

enum class Type { Colonist, Army };

struct Component {
  std::string name;
  Type type;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( name, type );
  }
};

};// namespace Actor
