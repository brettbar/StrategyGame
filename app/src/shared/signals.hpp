#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the Game State to the UI
namespace Messages {

  enum class Type {
    Basic,
    FactionSelected,
  };

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

// Used for sending data from the UI to the Game State
namespace Events {

  enum class Type {
    Basic,
    ButtonClick,
    JoinLobby,
  };

  struct Basic {
    std::string origin_id;
    Type type = Type::Basic;
  };

  struct ButtonClick : Basic {
    std::string msg;
    Type type = Type::ButtonClick;
  };

  struct JoinLobby : Basic {
    CSteamID lobby_id;
    Type type = Type::JoinLobby;
  };


  struct EventEmitter : entt::emitter<EventEmitter> {};

  inline EventEmitter event_emitter;

};// namespace Events
