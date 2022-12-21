//
// Created by brett on 12/31/2021.
//


#pragma once

#include "../common.hpp"


namespace Sight {

struct Component {
  u32 range;


  template<class Archive>
  void serialize( Archive &ar ) {
    ar ( range );
  }
};

}// namespace Component
