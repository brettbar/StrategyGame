// TODO call leave lobby in CLient and Host when appropriate

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

  // inline CSteamID lobby_id;

  struct ClientConnectionData {
    const char *player_id;

    bool active;
    CSteamID steam_user_id;
    // uint64 tick_count_last_data;
    HSteamNetConnection conn;

    ClientConnectionData() {
      player_id = "";
      active = false;
      conn = 0;
    }
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
    // SteamNetworkingUtils()->SetGlobalConfigValueInt32(
    //   k_ESteamNetworkingConfig_IP_AllowWithoutAuth, 2
    // );
  }

  inline void
  SendMessageOnConnection( HSteamNetConnection conn, const char *msg ) {
    printf( "Sending msg '%s'\n", msg );

    EResult r = SteamNetworkingSockets()->SendMessageToConnection(
      conn,
      msg,
      (int) strlen( msg ) + 1,
      k_nSteamNetworkingSend_Reliable,
      nullptr
    );

    assert( r == k_EResultOK );
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
