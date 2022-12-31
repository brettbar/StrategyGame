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

struct Network {

  STEAM_CALLBACK(
    Network,
    OnNetConnectionStatusChanged,
    SteamNetConnectionStatusChangedCallback_t
  );

  HSteamListenSocket socket;


  static void DebugOutput(
    ESteamNetworkingSocketsDebugOutputType eType,
    const char *pszMsg
  ) {
    printf( "%s\n", pszMsg );
  }


  void Host() {
    SteamNetworkingIdentity identity_local;
    identity_local.Clear();
    identity_local.SetLocalHost();

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    socket = SteamNetworkingSockets()->CreateListenSocketP2P( 0, 0, nullptr );


    // HSteamNetPollGroup poll_grp = SteamNetworkingSockets()->CreatePollGroup();
  }

  void Client() {
    SteamNetworkingIdentity identity_remote;
    identity_remote.Clear();
    identity_remote.SetLocalHost();

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    HSteamNetConnection conn =
      SteamNetworkingSockets()->ConnectP2P( identity_remote, 0, 0, nullptr );
  }

  void Exit() {
    printf( "Closing socket\n" );
    SteamNetworkingSockets()->CloseListenSocket( socket );
  }
};

inline void Network::OnNetConnectionStatusChanged(
  SteamNetConnectionStatusChangedCallback_t *pCallback
) {
  printf( "OnNetConnectionStatusChanged()!!!" );
  HSteamNetConnection conn = pCallback->m_hConn;
  SteamNetConnectionInfo_t info = pCallback->m_info;
  ESteamNetworkingConnectionState old_state = pCallback->m_eOldState;
  ESteamNetworkingConnectionState new_state = info.m_eState;

  if ( info.m_hListenSocket && old_state == k_ESteamNetworkingConnectionState_None && new_state == k_ESteamNetworkingConnectionState_Connecting ) {

  }
}