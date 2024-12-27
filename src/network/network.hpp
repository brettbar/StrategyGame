// TODO call leave lobby in Client and Host when appropriate

#pragma once

#include "../shared/common.hpp"

#include <steam/isteammatchmaking.h>
#include <steam/steam_api.h>
#include <steam/steam_api_common.h>
#include <steam/steamclientpublic.h>
#include <steam/steamnetworkingtypes.h>

#include <assert.h>
#include <chrono>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>

namespace Network {

  inline const uint32 MAX_PLAYERS_PER_SERVER = 8;

  // TODO this should have a better system later, but for now it will do
  inline const bool LOCAL = true;

  inline bool is_host = false;

  inline CSteamID lobby_id;

  // Peak programming right here
  inline std::map<std::string, u32> player_id_index = {
    { "player_0", 0 },
    { "player_1", 1 },
    { "player_2", 2 },
    { "player_3", 3 },
    { "player_4", 4 },
    { "player_5", 5 },
    { "player_6", 6 },
    { "player_7", 7 },
  };

  enum MessageID : u32 {
    InitiateContact,
    AssignedPlayerId,
    HostPingRequest,
    HostStartedCampaign,
    ClientPingResponse,
    PlayerConnected,
    PlayerDisconnected,

    PlayerToggledReady,

    PlayerFactionSelect,

    Command,

    NumMessageIDs,
  };

  struct Message {
    MessageID message_id;
    nlohmann::json body;
  };

  struct PeerData {
    std::string player_id;
    std::string faction = "";
    bool active = false;
    bool readied_up = false;
    CSteamID steam_user_id;
    // uint64 tick_count_last_data;
  };


  inline void DebugOutput(
    ESteamNetworkingSocketsDebugOutputType eType,
    const char *pszMsg
  ) {
    printf( "%s\n", pszMsg );
  }

  inline void Setup() {
    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    // TODO see if I can remove this
    // Remove auth
    SteamNetworkingUtils()->SetGlobalConfigValueInt32(
      k_ESteamNetworkingConfig_IP_AllowWithoutAuth, 2
    );
  }

  inline void SendMessageOnConnection(
    HSteamNetConnection conn,
    Message message
    // const char *msg
  ) {
    // printf( "Sending msg '%s'\n", payload );

    nlohmann::json message_payload = {
      {
        "message_id",
        message.message_id,
      },
      { "body", message.body },
    };


    EResult r = SteamNetworkingSockets()->SendMessageToConnection(
      conn,
      message_payload.dump().c_str(),
      (int) strlen( message_payload.dump().c_str() ) + 1,
      k_nSteamNetworkingSend_Reliable,
      nullptr
    );

    // printf( "Message Result %d\n", r );

    // TODO this really should be enabled, it broke linux build
    // for some reason
    // assert( r == k_EResultOK );
  }

  // inline std::vector<CSteamID> GetLobbyMembers() {
  //   std::vector<CSteamID> members = {};

  //   for ( uint32 i = 0; i < SteamMatchmaking()->GetNumLobbyMembers( lobby_id );
  //         i++ ) {
  //     members.push_back(
  //       SteamMatchmaking()->GetLobbyMemberByIndex( lobby_id, i )
  //     );
  //   }

  //   return members;
  // }


};// namespace Network
