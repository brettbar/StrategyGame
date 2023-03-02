#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the Game State to the UI
namespace Messages
{
  enum Type : u32
  {
    EnabledUpdate,
    TextUpdate,
    BackgroundUpdate,

    TargetedTextUpdate,
    TargetedBackgroundUpdate,

    NumTypes,
  };

  enum class ID
  {
    FactionSelected,
    SettlementContext,
    ActorContext,
    JoinLobby,
    HostLobby,
  };

  struct DataUnion
  {
    Type type;
    ID message_id;
    //
    bool on;
    std::string updated_text;
    Color updated_background;
    std::string target;

    // TextUpdate
    DataUnion( Type type, ID message_id, std::string updated_text )
        : type( type ), message_id( message_id ), updated_text( updated_text )
    {
    }

    // TargetedTextUpdate
    DataUnion(
      Type type,
      ID message_id,
      std::string target,
      std::string updated_text
    )
        : type( type ), message_id( message_id ), target( target ),
          updated_text( updated_text )
    {
    }

    // EnabledUpdate
    DataUnion( Type type, ID message_id, bool on )
        : type( type ), message_id( message_id ), on( on )
    {
    }

    // TargetedBackgroundUpdate
    DataUnion(
      Type type,
      ID message_id,
      std::string target,
      Color updated_background
    )
        : type( type ), message_id( message_id ), target( target ),
          updated_background( updated_background )
    {
    }


    // BackgroundUpdate
    DataUnion( Type type, ID message_id, Color updated_background )
        : type( type ), message_id( message_id ),
          updated_background( updated_background )
    {
    }
  };

  inline entt::dispatcher dispatcher{};
};// namespace Messages

// Used for sending data from the UI to the Game State
namespace Events
{
  // TODO replace with ID + string?
  enum ID : u32
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
  inline const char *const IDString[ID::NumberOfEvents] = {
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


  struct EventUnion
  {
    ID id;

    std::string msg;
    CSteamID lobby_id;

    std::string source;

    EventUnion( ID id ) : id( id ) {}
    EventUnion( ID id, std::string msg ) : id( id ), msg( msg ) {}
    EventUnion( ID id, std::string msg, std::string source )
        : id( id ), msg( msg ), source( source )
    {
    }
    EventUnion( ID id, std::string msg, CSteamID lobby_id )
        : id( id ), msg( msg ), lobby_id( lobby_id )
    {
    }
  };

  struct EventEmitter : entt::emitter<EventEmitter>
  {
  };

  inline EventEmitter event_emitter;

};// namespace Events
