#pragma once

#include "steam/isteammatchmaking.h"
#include "steam/steam_api_common.h"
#include "steam/steamclientpublic.h"
#include "steam/steamnetworkingtypes.h"
#include <steam/steam_api.h>

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

static void DebugOutput(
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

  // Remove auth
  SteamNetworkingUtils()->SetGlobalConfigValueInt32(
    k_ESteamNetworkingConfig_IP_AllowWithoutAuth, 2
  );
}

inline void SendMessageToPeer( HSteamNetConnection conn, const char *msg ) {
  printf( "Sending msg '%s'\n", msg );

  EResult r = SteamNetworkingSockets()->SendMessageToConnection(
    conn, msg, (int) strlen( msg ) + 1, k_nSteamNetworkingSend_Reliable, nullptr
  );

  assert( r == k_EResultOK );
}

struct ClientConnectionData {
  bool active;
  CSteamID steam_user_id;
  // uint64 tick_count_last_data;
  HSteamNetConnection conn;

  ClientConnectionData() {
    active = false;
    conn = 0;
  }
};


// struct Client {
//   STEAM_CALLBACK(
//     Client,
//     OnClientNetConnectionStatusChanged,
//     SteamNetConnectionStatusChangedCallback_t
//   );

//   HSteamListenSocket socket;
//   HSteamNetConnection conn;

//   void Run() {
//     SteamNetworkingIdentity identity_remote;


//     printf(
//       "Client:: My ID is %lld, I think the server's is %lld\n",
//       SteamUser()->GetSteamID().ConvertToUint64(),
//       identity_remote.GetSteamID64()
//     );

//     SteamNetworkingUtils()->SetDebugOutputFunction(
//       k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
//     );

//     conn =
//       SteamNetworkingSockets()->ConnectP2P( identity_remote, 0, 0, nullptr );

//     printf( "End of run...\n" );
//   }
// };


// void Exit() {
//   printf( "Closing socket\n" );
//   SteamNetworkingSockets()->CloseListenSocket( socket );
// }


// inline void Host::OnHostNetConnectionStatusChanged(
//   SteamNetConnectionStatusChangedCallback_t *pCallback
// ) {
//   printf( "Host::OnNetConnectionStatusChanged()!!!\n" );

//   HSteamNetConnection conn = pCallback->m_hConn;
//   SteamNetConnectionInfo_t info = pCallback->m_info;
//   ESteamNetworkingConnectionState old_state = pCallback->m_eOldState;
//   ESteamNetworkingConnectionState new_state = info.m_eState;

//   if ( info.m_hListenSocket && old_state == k_ESteamNetworkingConnectionState_None && new_state == k_ESteamNetworkingConnectionState_Connecting ) {
//     EResult res = SteamNetworkingSockets()->AcceptConnection( conn );
//     if ( res != k_EResultOK ) {
//       printf( "AcceptConnection returned %d", res );
//       SteamNetworkingSockets()->CloseConnection(
//         conn,
//         k_ESteamNetConnectionEnd_AppException_Generic,
//         "Failed to accept connecton",
//         false
//       );
//       return;
//     }

//     const char *msg = "Welcome to the server";

//     SteamNetworkingSockets()->SendMessageToConnection(
//       conn, msg, strlen( msg ), k_nSteamNetworkingSend_Reliable, nullptr
//     );
//   }
// }

// inline void Client::OnClientNetConnectionStatusChanged(
//   SteamNetConnectionStatusChangedCallback_t *pCallback
// ) {
//   printf( "Client::OnNetConnectionStatusChanged()!!!\n" );

//   HSteamNetConnection conn = pCallback->m_hConn;
//   SteamNetConnectionInfo_t info = pCallback->m_info;
//   ESteamNetworkingConnectionState old_state = pCallback->m_eOldState;
//   ESteamNetworkingConnectionState new_state = info.m_eState;
// }

};// namespace Network