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
    Type type;
    std::string dest_id;
    std::string msg;

    virtual ~Basic() {}

    Basic( std::string dest_id, std::string msg = "" )
        : dest_id( dest_id ), type( Type::Basic ) {}

    Basic( Type type, std::string dest_id, std::string msg = "" )
        : dest_id( dest_id ), msg( msg ), type( type ) {}
  };

  struct FactionSelected : Basic {
    Color color;

    std::shared_ptr<Messages::FactionSelected> static Create(
      std::string id,
      std::string msg
    ) {
      return std::make_shared<Messages::FactionSelected>( id, msg );
    }

    FactionSelected( std::string id, std::string faction )
        : Basic( Type::FactionSelected, id ) {
      // TODO replace with json stuff
      color = [&]() -> Color {
        if ( faction == "romans" )
          return RED;
        if ( faction == "greeks" )
          return BLUE;
        if ( faction == "celts" )
          return GREEN;
        if ( faction == "punics" )
          return PURPLE;
        if ( faction == "germans" )
          return GRAY;
        if ( faction == "scythians" )
          return PINK;
        if ( faction == "persians" )
          return ORANGE;
        else
          return BLACK;
      }();
    }
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
    Type type;
    std::string origin_id;

    virtual ~Basic() {}

    Basic( std::string origin_id )
        : origin_id( origin_id ), type( Type::Basic ) {}

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
