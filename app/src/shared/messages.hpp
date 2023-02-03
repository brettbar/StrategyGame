// Used for sending data from the Game State to the UI
#pragma once

#include "common.hpp"

namespace Messages {

  struct UIUpdate {
    std::string dest_id;
    std::string updated_value;
  };

  struct FactionSelected : UIUpdate {
    Color color;
  };

  struct MessageEmitter : entt::emitter<MessageEmitter> {};

  inline MessageEmitter message_emitter;

};// namespace Messages
