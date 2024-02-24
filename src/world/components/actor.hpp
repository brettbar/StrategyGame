//
// Created by brett on 1/30/2022.
//

#pragma once

#include "../../shared/common.hpp"

namespace Actor {

  enum class Type {
    Colonist,
    Diplomat,
    Explorer,
    Merchant,

    ArmyTierI,


    Special,
  };

  inline map<str, Type> type_lookup = {
    { "colonist", Type::Colonist },
    { "diplomat", Type::Diplomat },
    { "explorer", Type::Explorer },
    { "merchant", Type::Merchant },
    { "army_tier_i", Type::ArmyTierI },
    { "special", Type::Special },
  };

  struct Data {
    Type type;// Type::Colonist

    str actor_id;  // romans_colonist
    str name;      // plebeian
    str faction_id;// romans
    hstr sprite_id;//  hstr{"romans_plebeian"}

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( type, actor_id, faction_id, sprite_id );
    }
  };

  struct Component {
    entt::entity owner;
    Data data;
    vec2f position;
    vec2f destination;
    f32 speed;
    bool selected = false;
    bool moving = false;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( owner, position.x, position.y, destination.x, destination.y, speed );
      data.serialize( ar );
    }
  };

};// namespace Actor
