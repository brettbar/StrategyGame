#pragma once

#include "../../shared/common.hpp"

#include <cereal/types/common.hpp>

namespace Player {

  struct LocalTag {};
  struct RemoteTag {};

  struct Component {
    entt::entity id;
    std::string player_id;
    bool is_human;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( CEREAL_NVP(id), CEREAL_NVP(player_id), is_human );
    }
  };

};// namespace Player
