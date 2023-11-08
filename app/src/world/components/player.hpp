#pragma once

#include "../../shared/common.hpp"

namespace Player
{

  struct LocalTag
  {
  };
  struct RemoteTag
  {
  };

  struct Component
  {
    entt::entity id;
    std::string player_id;
    bool is_human;

    template<class Archive>
    void serialize( Archive &ar )
    {
      ar( id, player_id, is_human );
    }
  };

};// namespace Player
