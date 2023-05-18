#pragma once

#include "../shared/common.hpp"
#include <steam/steamclientpublic.h>


// Used for sending data from the Game State to the UI
namespace InterfaceUpdate
{
  namespace
  {
    inline entt::dispatcher dispatcher{};
  };

  enum ID
  {
    INVALID,
    PlayerSelectedFaction,
    PlayerJoinedLobby,

    SettlementContext,
    SettlementPopUpdated,
    SettlementContextPopulationTab,
    SettlementContextResourcesTab,

    ActorContext,
    ActorCanSpawnSettlement,
    JoinLobby,
    HostLobby,
    PlayerToggledReady,
    AllPlayersReady,
    NumIDs,
  };

  struct Update
  {
    ID id = ID::INVALID;
    std::string update_txt = "INVALID";
    std::string player_id = "INVALID";
    bool condition = false;

    nlohmann::json json;

    void Send()
    {
      dispatcher.enqueue( *this );
    }
  };

};// namespace InterfaceUpdate
