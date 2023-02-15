#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the Game State to the UI
namespace Messages {

  enum class ID {
    FactionSelected,
    SettlementContext,
    ActorContext,
  };

  struct UpdateEnabled {
    ID message_id;
    bool on;
  };

  struct UpdateText {
    ID message_id;
    std::string updated_text;

    UpdateText( ID message_id, std::string updated_text )
        : message_id( message_id ), updated_text( updated_text ) {}
  };

  struct UpdateBackground {
    ID message_id;
    Color updated_background;

    UpdateBackground( ID message_id, Color updated_background )
        : message_id( message_id ), updated_background( updated_background ) {}
  };

  // struct FactionSelected : Basic {
  //   Color color;

  //   std::shared_ptr<Messages::FactionSelected> static Create(
  //     std::string id,
  //     std::string msg
  //   ) {
  //     return std::make_shared<Messages::FactionSelected>( id, msg );
  //   }

  //   FactionSelected( std::string id, std::string faction )
  //       : Basic( Type::FactionSelected, id ) {
  //     }();
  //   }
  // };

  // struct MessageEmitter : entt::emitter<MessageEmitter> {};

  // inline MessageEmitter message_emitter;
  inline entt::dispatcher dispatcher{};
};// namespace Messages

// Used for sending data from the UI to the Game State
namespace Events {

  enum class Type {
    Basic,
    ButtonClick,
    JoinLobby,
  };

  struct Basic {
    Type type;
    std::string origin_id;

    virtual ~Basic() {}

    Basic( std::string origin_id )
        : type( Type::Basic ), origin_id( origin_id ) {}

    Basic( Type type, std::string origin_id )
        : type( type ), origin_id( origin_id ) {}
  };

  struct ButtonClick : Basic {
    std::string msg;

    std::shared_ptr<Events::ButtonClick> static Create(
      std::string id,
      std::string msg
    ) {
      return std::make_shared<Events::ButtonClick>( id, msg );
    }

    ButtonClick( std::string origin_id, std::string msg )
        : Basic( Type::ButtonClick, origin_id ), msg( msg ) {}
  };

  struct JoinLobby : Basic {
    CSteamID lobby_id;

    JoinLobby( std::string origin_id, CSteamID lobby_id )
        : Basic( Type::JoinLobby, origin_id ), lobby_id( lobby_id ) {}
  };


  struct EventEmitter : entt::emitter<EventEmitter> {};

  inline EventEmitter event_emitter;

};// namespace Events
