// Used for sending data from the UI to the Game State
#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

namespace Events {

  struct ButtonClick {
    std::string origin_id;
    std::string msg;
  };

  struct JoinLobby {
    std::string origin_id;
    CSteamID lobby_id;
  };


  struct EventEmitter : entt::emitter<EventEmitter> {};

  inline EventEmitter event_emitter;

};// namespace Events


// Used for sending data from the Game State to the UI
namespace Messages {

  struct Basic {
    std::string dest_id;
    std::string updated_value;
  };

  struct FactionSelected : Basic {
    Color color;
  };

  struct MessageEmitter : entt::emitter<MessageEmitter> {};

  inline MessageEmitter message_emitter;

};// namespace Messages
