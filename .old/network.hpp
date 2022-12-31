#include "steam/isteamfriends.h"
#include "steam/isteammatchmaking.h"
#include "steam/isteamnetworkingutils.h"
#include "steam/steam_api_common.h"
#include "steam/steamclientpublic.h"
#include "steam/steamnetworkingtypes.h"
#include <steam/steam_api.h>


#include <assert.h>
#include <stdlib.h>


#include <stdarg.h>
#include <stdio.h>


#include <chrono>
#include <iostream>
#include <thread>


namespace Network {


struct Client {
  CSteamID lobby_id;
  int virtual_port = 0;

  SteamNetworkingIdentity identity_local;
  SteamNetworkingIdentity identity_remote;
  inline static HSteamListenSocket listen_socket;
  inline static HSteamNetConnection conn;

  static void Quit( int rc ) {
    if ( rc == 0 ) {
      // OK, we cannot just exit the process, because we need to give
      // the connection time to actually send the last message and clean up.
      // If this were a TCP connection, we could just bail, because the OS
      // would handle it.  But this is an application protocol over UDP.
      // So give a little bit of time for good cleanup.  (Also note that
      // we really ought to continue pumping the signaling service, but
      // in this exampple we'll assume that no more signals need to be
      // exchanged, since we've gotten this far.)  If we just terminated
      // the program here, our peer could very likely timeout.  (Although
      // it's possible that the cleanup packets have already been placed
      // on the wire, and if they don't drop, things will get cleaned up
      // properly.)
      printf( "Waiting for any last cleanup packets.\n" );
      std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    exit( rc );
  }

  static void SendMessageToPeer( const char *pszMsg ) {
    printf( "Sending msg '%s'\n", pszMsg );
    EResult r = SteamNetworkingSockets()->SendMessageToConnection(
      conn,
      pszMsg,
      (int) strlen( pszMsg ) + 1,
      k_nSteamNetworkingSend_Reliable,
      nullptr
    );
    assert( r == k_EResultOK );
  }

  static void
  DebugOutput( ESteamNetworkingSocketsDebugOutputType eType, const char *msg ) {
    printf( "%s\n", msg );
  }

  static void OnSteamNetConnectionStatusChanged(
    SteamNetConnectionStatusChangedCallback_t *info
  ) {
    switch ( info->m_info.m_eState ) {
      case k_ESteamNetworkingConnectionState_Connecting: {
        // Is this a connection we initiated, or one that we are receiving?
        if ( listen_socket != k_HSteamListenSocket_Invalid && info->m_info.m_hListenSocket == listen_socket ) {

          // Somebody's knocking
          // Note that we assume we will only ever receive a single connection
          assert(
            conn == k_HSteamNetConnection_Invalid
          );// not really a bug in this code, but a bug in the test

          printf( "[%s] Accepting\n", info->m_info.m_szConnectionDescription );
          conn = info->m_hConn;
          SteamNetworkingSockets()->AcceptConnection( info->m_hConn );
        }
        else {
          // Note that we will get notification when our own connection that
          // we initiate enters this state.
          assert( conn == info->m_hConn );
          printf(
            "[%s] Entered connecting state\n",
            info->m_info.m_szConnectionDescription
          );
        }
      } break;
      case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
        printf(
          "[%s] %s, reason %d: %s\n",
          info->m_info.m_szConnectionDescription,
          ( info->m_info.m_eState ==
                k_ESteamNetworkingConnectionState_ClosedByPeer
              ? "closed by peer"
              : "problem detected locally" ),
          info->m_info.m_eEndReason,
          info->m_info.m_szEndDebug
        );

        // Close our end
        SteamNetworkingSockets()->CloseConnection(
          info->m_hConn, 0, nullptr, false
        );

        if ( conn == info->m_hConn ) {
          conn = k_HSteamNetConnection_Invalid;

          // In this example, we will bail the test whenever this happens.
          // Was this a normal termination?
          int rc = 0;
          if ( rc == k_ESteamNetworkingConnectionState_ProblemDetectedLocally || info->m_info.m_eEndReason != k_ESteamNetConnectionEnd_App_Generic )
            rc = 1;// failure
          Quit( rc );
        }
        else {
          // Why are we hearing about any another connection?
          assert( false );
        }
      } break;
    }
  }

  static inline void Listen() {
    for ( ;; ) {
      SteamNetworkingSockets()->RunCallbacks();
      std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );

      if ( conn != k_HSteamNetConnection_Invalid ) {
        SteamNetworkingMessage_t *msg;
        int r = SteamNetworkingSockets()->ReceiveMessagesOnConnection(
          conn, &msg, 1
        );
        assert( r == 0 || r == 1 );
        if ( r == 1 ) {
          printf( "Received message '%s'\n", (const char *) msg->GetData() );

          msg->Release();

          // We're the server.  Send a reply.
          SendMessageToPeer( "I got your message" );
        }
      }
    }
  }


  Client() {
    identity_local.Clear();
    identity_remote.Clear();

    SteamNetworkingUtils()->InitRelayNetworkAccess();

    SteamNetworkingUtils()->SetDebugOutputFunction(
      k_ESteamNetworkingSocketsDebugOutputType_Debug, DebugOutput
    );

    // This causes the dll error
    // SteamNetworkingUtils()->SetGlobalConfigValueInt32(
    //   k_ESteamNetworkingConfig_LogLevel_P2PRendezvous,
    //   k_ESteamNetworkingSocketsDebugOutputType_Debug
    // );

    // Init here

    // SteamNetworkingUtils()->SetGlobalConfigValueString(
    //   k_ESteamNetworkingConfig_P2P_STUN_ServerList, "stun.l.google.com:19302"
    // );

    SteamNetworkingUtils()->SetGlobalConfigValueInt32(
      k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable,
      k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_All
    );

    SteamNetworkingUtils()->SetGlobalConfigValueInt32(
      k_ESteamNetworkingConfig_IP_AllowWithoutAuth, 2
    );

    SteamNetworkingUtils()->SetGlobalCallback_SteamNetConnectionStatusChanged(
      OnSteamNetConnectionStatusChanged
    );

    printf(
      "Connecting to '%s', virtual port %d, from local virtual port %d.\n",
      SteamNetworkingIdentityRender( identity_remote ).c_str(),
      0,
      0
    );
  }

  void Host() {
    const char *local_name = "str:peer_server";

    if ( !identity_local.ParseString( local_name ) ) {
      printf( "%s is not a valid identity string\n", local_name );
      return;
    }

    listen_socket = SteamNetworkingSockets()->CreateListenSocketP2P(
      virtual_port, 0, nullptr
    );
    assert( listen_socket != k_HSteamListenSocket_Invalid );

    Listen();


    // SteamNetworkingSockets()->CreatePollGroup();
  }

  void Join() {
    const char *local_name = "str:peer_client";
    const char *remote_name = "str:peer_server";

    if ( !identity_remote.ParseString( remote_name ) ) {
      printf( "%s is not a valid identity string\n", remote_name );
      return;
    }

    conn = SteamNetworkingSockets()->ConnectP2P(
      identity_remote, virtual_port, 0, nullptr
    );
  }
};


};// namespace Network
