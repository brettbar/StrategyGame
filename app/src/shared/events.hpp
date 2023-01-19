#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

namespace Events {

  struct ButtonClick {
    std::string origin;
    std::string msg;
  };

  struct JoinLobby {
    std::string origin;
    CSteamID lobby_id;
  };


  struct EventEmitter : entt::emitter<EventEmitter> {};

  inline EventEmitter event_emitter;

};// namespace Events
