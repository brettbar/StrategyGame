//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../../shared/common.hpp"

namespace Actor
{

  enum class Type
  {
    Colonist,
    Army
  };

  struct Component
  {

    std::string name;
    Type type;
    entt::entity owner;
    Vector2 position;
    Vector2 destination;
    f32 speed;
    bool selected = false;
    bool moving = false;

    template<class Archive>
    void serialize( Archive &ar )
    {
      ar(
        name,
        type,
        owner,
        position.x,
        position.y,
        destination.x,
        destination.y,
        speed
      );
    }
  };

};// namespace Actor
