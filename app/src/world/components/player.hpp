#pragma once

#include "../../shared/common.hpp"

namespace Player {

struct Component {
  entt::entity id;
  bool is_human;
  std::string faction;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( id, is_human, faction );
  }
};

};// namespace Player
