#pragma once

#include "common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the Game State to the UI
namespace InterfaceUpdate
{


  /*
     Time to figure out this signal business.
     For InterfaceUpdates we have 2 ideas.

InterfaceUpdates:
     1. can be intended for one to many elements
     2. can be intended for one element only
     3. can update enabled
     4. can update text 
     5. can update background
     6. can update clickable


 */


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

  struct Data
  {
    Type type;
    ID update_id;
    //
    bool on;
    std::string updated_text;
    Color updated_background;
    std::string target;

    // TextUpdate
    Data( Type type, ID message_id, std::string updated_text )
        : type( type ), update_id( message_id ), updated_text( updated_text )
    {
    }

    // TargetedTextUpdate
    Data(
      Type type,
      ID message_id,
      std::string target,
      std::string updated_text
    )
        : type( type ), update_id( message_id ), updated_text( updated_text ),
          target( target )
    {
    }

    // EnabledUpdate
    Data( Type type, ID message_id, bool on )
        : type( type ), update_id( message_id ), on( on )
    {
    }

    // TargetedBackgroundUpdate
    Data(
      Type type,
      ID message_id,
      std::string target,
      Color updated_background
    )
        : type( type ), update_id( message_id ),
          updated_background( updated_background ), target( target )
    {
    }


    // BackgroundUpdate
    Data( Type type, ID message_id, Color updated_background )
        : type( type ), update_id( message_id ),
          updated_background( updated_background )
    {
    }
  };

  inline entt::dispatcher dispatcher{};
};// namespace InterfaceUpdate

// Used for sending data from the UI to the Game State
namespace InterfaceEvent
{
  // TODO replace with ID + string? or something categorized
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

  struct EventEmitter : entt::emitter<EventEmitter>
  {
  };

  inline EventEmitter event_emitter;

};// namespace InterfaceEvent
