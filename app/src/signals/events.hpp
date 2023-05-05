#pragma once

#include "../shared/common.hpp"
#include <steamclientpublic.h>

// Used for sending data from the UI to the Game State
namespace InterfaceEvent
{
  // TODO replace with ID + string? or something categorized
  enum class ID : u32
  {
    MainMenuHostGame,
    MainMenuJoinGame,
    MainMenuStartGame,
    MainMenuLoadGame,
    MainMenuSettings,
    MainMenuExitGame,
    OpenFactionSelectPage,
    SinglePlayerLobbyStartGame,
    ModalMenuLoadGame,
    ModalMenuSaveGame,
    ModalMenuSettings,
    ModalMenuSaveExitMain,
    ModalMenuExitMain,
    ModalMenuExitGame,
    ModalMenuToggle,
    ReturnToMain,
    PlayerToggledReady,
    HostStartGame,
    JoinLobby,
    JoinHostedCampaign,
    FactionSelected,


    ActorSpawnSettlment,

    // NOTE: Final Enum used to get count, must be in last place
    NumberOfEvents
  };
  // TODO this is a disaster waiting to happen,
  // Do something other than this
  //
  // This needs to be in the same order as the ID enum
  inline const char *const IDString[(u32) ID::NumberOfEvents] = {
    "MainMenuHostGame",      "MainMenuJoinGame",
    "MainMenuStartGame",     "MainMenuLoadGame",
    "MainMenuSettings",      "MainMenuExitGame",

    "OpenFactionSelectPage", "SinglePlayerLobbyStartGame",

    "ModalMenuLoadGame",     "ModalMenuSaveGame",
    "ModalMenuSettings",     "ModalMenuSaveExitMain",
    "ModalMenuExitMain",     "ModalMenuExitGame",
    "ModalMenuToggle",

    "ReturnToMain",          "PlayerToggledReadyUp",
    "HostStartGame",         "JoinLobby",

    "JoinHostedCampaign",

    "FactionSelected",

    "ActorSpawnSettlment",
    //
  };


  struct Data
  {
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

public:
    ID event_id;
    std::string msg;
    CSteamID lobby_id;
    std::string source;
  };

  struct EventEmitter : entt::emitter<EventEmitter>
  {
  };

  inline EventEmitter event_emitter;

};// namespace InterfaceEvent
