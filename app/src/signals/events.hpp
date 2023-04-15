#pragma once

#include "../shared/common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the UI to the Game State
namespace InterfaceEvent
{
  // TODO replace with ID + string? or something categorized
  enum class ID : u32
  {
    // BASIC
    // MainMenu
    MainMenuHostGame,
    MainMenuJoinGame,
    MainMenuStartGame,
    MainMenuLoadGame,
    MainMenuSettings,
    MainMenuExitGame,
    // FactionSelect
    OpenFactionSelectPage,
    SinglePlayerLobbyStartGame,
    // ModalMenu
    ModalMenuLoadGame,
    ModalMenuSaveGame,
    ModalMenuSettings,
    ModalMenuSaveExitMain,
    ModalMenuExitMain,
    ModalMenuExitGame,
    ModalMenuToggle,
    // ??
    ReturnToMain,
    ReadyUp,
    JoinLobby,

    // STRING
    FactionSelected,


    // Context
    ActorSpawnSettlment,


    // NOTE: Final Enum used to get count, must be in last place
    NumberOfEvents
  };
  // This needs to be in the same order as the ID enum
  inline const char *const IDString[(u32) ID::NumberOfEvents] = {
    "MainMenuHostGame",
    "MainMenuJoinGame",
    "MainMenuStartGame",
    "MainMenuLoadGame",
    "MainMenuSettings",
    "MainMenuExitGame",

    "OpenFactionSelectPage",
    "SinglePlayerLobbyStartGame",

    "ModalMenuLoadGame",
    "ModalMenuSaveGame",
    "ModalMenuSettings",
    "ModalMenuSaveExitMain",
    "ModalMenuExitMain",
    "ModalMenuExitGame",
    "ModalMenuToggle",

    "ReturnToMain",
    "ReadyUp",
    "JoinLobby",

    "FactionSelected",

    "ActorSpawnSettlment",
  };


  struct Data
  {
    ID event_id;

    std::string msg;
    CSteamID lobby_id;

    std::string source;

    Data( ID id ) : event_id( id ) {}
    Data( ID id, std::string msg ) : event_id( id ), msg( msg ) {}
    Data( ID id, std::string msg, std::string source )
        : event_id( id ), msg( msg ), source( source )
    {
    }
    Data( ID id, std::string msg, CSteamID lobby_id )
        : event_id( id ), msg( msg ), lobby_id( lobby_id )
    {
    }
  };

  struct Builder
  {
protected:
    Data _data;

public:
    Data build()
    {
      return _data;
    }
  };

  struct EventEmitter : entt::emitter<EventEmitter>
  {
  };

  inline EventEmitter event_emitter;

};// namespace InterfaceEvent
