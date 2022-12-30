#include <steam/isteammatchmaking.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steam_api.h>
#include <steam/steamnetworkingtypes.h>


#include <assert.h>

#include <stdarg.h>
#include <stdio.h>


#include <iostream>


namespace Network {


// typedef void (*FSteamNetworkingSocketsDebugOutput)( ESteamNetworkingSocketsDebugOutputType nType, const char *pszMsg );

struct Host {
  SteamNetworkingIdentity identity_local;
  int virtual_port = 0;

  inline static void DebugOutput(
    ESteamNetworkingSocketsDebugOutputType eType,
    const char *pszMsg
  ) {
    printf( "Network:: %s", pszMsg );
    std::cout << "Network::" << pszMsg << std::endl;
    assert( !"TEST FAILED" );
  }


  inline void Setup() {
    identity_local.Clear();


    printf( "%lldx\n", SteamNetworkingUtils()->GetLocalTimestamp() );


    // SteamNetworkingUtils()->InitRelayNetworkAccess();

    // SteamNetworkingUtils()->SetDebugOutputFunction(
    //   k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    // );

    // SteamNetworkingUtils()->SetGlobalConfigValueInt32(
    //   k_ESteamNetworkingConfig_LogLevel_P2PRendezvous,
    //   k_ESteamNetworkingSocketsDebugOutputType_Debug
    // );

    // SteamNetworkingUtils()->SetGlobalConfigValueString(
    //   k_ESteamNetworkingConfig_P2P_STUN_ServerList, "stun.l.google.com:19302"
    // );

    // SteamNetworkingUtils()->SetGlobalConfigValueInt32(
    //   k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable,
    //   k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_All
    // );


    // auto socket = SteamNetworkingSockets()->CreateListenSocketP2P(
    //   virtual_port, 0, nullptr
    // );
  }
};

};// namespace Network
