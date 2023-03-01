#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the Game State to the UI
namespace Messages {

  enum class ID {
    FactionSelected,
    SettlementContext,
    ActorContext,
    JoinLobby,
    HostLobby,
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

  inline entt::dispatcher dispatcher{};
};// namespace Messages

// Used for sending data from the UI to the Game State
namespace Events {

  enum class Type {
    Basic,
    ButtonClick,
    JoinLobby,
  };

  // TODO replace with ID + string?
  enum class ID {
    // MainMenu
    MainMenuHostGame,
    MainMenuJoinGame,
    MainMenuStartGame,
    MainMenuLoadGame,
    MainMenuExitGame,
    // FactionSelect
    OpenFactionSelectPage,
    SinglePlayerLobbyStartGame,
    // ModalMenu
    ModalMenuLoadGame,
    ModalMenuSaveGame,
    ModalMenuExitMain,
    ModalMenuExitGame,
    ModalMenuToggle,
    // ??
    ReturnToMain,
    ReadyUp,
  };

  struct Basic {
    Type type;
    // std::string origin_id;
    ID id;

    virtual ~Basic() {}

    Basic( ID id ) : type( Type::Basic ), id( id ) {}

    Basic( Type type, ID id ) : type( type ), id( id ) {}
  };

  struct ButtonClick : Basic {
    std::string msg;

    std::shared_ptr<Events::ButtonClick> static Create(
      std::string id,
      std::string msg
    ) {
      return std::make_shared<Events::ButtonClick>( id, msg );
    }

    ButtonClick( ID id, std::string msg )
        : Basic( Type::ButtonClick, id ), msg( msg ) {}
  };

  struct JoinLobby : Basic {
    CSteamID lobby_id;

    JoinLobby( ID id, CSteamID lobby_id )
        : Basic( Type::JoinLobby, id ), lobby_id( lobby_id ) {}

    std::shared_ptr<Events::JoinLobby> static Create(
      std::string id,
      CSteamID lobby_id
    ) {
      return std::make_shared<Events::JoinLobby>( id, lobby_id );
    }
  };


  struct EventEmitter : entt::emitter<EventEmitter> {};

  inline EventEmitter event_emitter;

};// namespace Events
