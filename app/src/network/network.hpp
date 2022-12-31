#pragma once

#include "steam/steam_api_common.h"
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

static void DebugOutput(
  ESteamNetworkingSocketsDebugOutputType eType,
  const char *pszMsg
) {
  printf( "%s\n", pszMsg );
}

struct Host {
  STEAM_CALLBACK(
    Host,
    OnHostNetConnectionStatusChanged,
    SteamNetConnectionStatusChangedCallback_t
  );

  HSteamListenSocket socket;
  HSteamNetConnection conn;


  void Run() {
    SteamNetworkingIdentity identity_local;
    identity_local.Clear();
    identity_local.ParseString( "str:peer_server" );

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    socket = SteamNetworkingSockets()->CreateListenSocketP2P( 0, 0, nullptr );


    // HSteamNetPollGroup poll_grp = SteamNetworkingSockets()->CreatePollGroup();
  }
};

struct Client {
  STEAM_CALLBACK(
    Client,
    OnClientNetConnectionStatusChanged,
    SteamNetConnectionStatusChangedCallback_t
  );

  HSteamListenSocket socket;
  HSteamNetConnection conn;

  void Run() {
    SteamNetworkingIdentity identity_remote;
    identity_remote.Clear();
    identity_remote.ParseString( "str:peer_server" );

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    conn =
      SteamNetworkingSockets()->ConnectP2P( identity_remote, 0, 0, nullptr );
  }
};


// void Exit() {
//   printf( "Closing socket\n" );
//   SteamNetworkingSockets()->CloseListenSocket( socket );
// }


inline void Host::OnHostNetConnectionStatusChanged(
  SteamNetConnectionStatusChangedCallback_t *pCallback
) {
  printf( "Host::OnNetConnectionStatusChanged()!!!\n" );

  HSteamNetConnection conn = pCallback->m_hConn;
  SteamNetConnectionInfo_t info = pCallback->m_info;
  ESteamNetworkingConnectionState old_state = pCallback->m_eOldState;
  ESteamNetworkingConnectionState new_state = info.m_eState;

  if ( info.m_hListenSocket && old_state == k_ESteamNetworkingConnectionState_None && new_state == k_ESteamNetworkingConnectionState_Connecting ) {
    EResult res = SteamNetworkingSockets()->AcceptConnection( conn );
    if ( res != k_EResultOK ) {
      printf( "AcceptConnection returned %d", res );
      SteamNetworkingSockets()->CloseConnection(
        conn,
        k_ESteamNetConnectionEnd_AppException_Generic,
        "Failed to accept connecton",
        false
      );
      return;
    }

    const char *msg = "Welcome to the server";

    SteamNetworkingSockets()->SendMessageToConnection(
      conn, msg, strlen( msg ), k_nSteamNetworkingSend_Reliable, nullptr
    );
  }
}

inline void Client::OnClientNetConnectionStatusChanged(
  SteamNetConnectionStatusChangedCallback_t *pCallback
) {
  printf( "Client::OnNetConnectionStatusChanged()!!!\n" );

  HSteamNetConnection conn = pCallback->m_hConn;
  SteamNetConnectionInfo_t info = pCallback->m_info;
  ESteamNetworkingConnectionState old_state = pCallback->m_eOldState;
  ESteamNetworkingConnectionState new_state = info.m_eState;
}

};// namespace Network