#include <iostream>
#include <map>
#include <thread>

#include "GameNetworkingSockets/steam/steam_api_common.h"
#include <GameNetworkingSockets/steam/isteamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <GameNetworkingSockets/steam/steamnetworkingtypes.h>

static void SteamNetConnectionStatusChangedCallback(
  SteamNetConnectionStatusChangedCallback_t *pInfo ) {
  printf( "Connection Status changed.\n" );
}

struct Client_t {
  std::string name;
};


class Server {
  bool _quit = false;
  HSteamListenSocket _listen_socket;
  HSteamNetPollGroup _poll_grp;
  ISteamNetworkingSockets *_interface;
  std::map<HSteamNetConnection, Client_t> clients;


  void Run() {
    _interface = SteamNetworkingSockets();

    SteamNetworkingIPAddr server_local_addr;
    server_local_addr.Clear();
    char *addr = (char *) "127.0.0.1:1234";
    SteamNetworkingIPAddr_ToString(
      &server_local_addr,
      addr,
      strlen( addr ),
      true );

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(
      k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
      (void *) SteamNetConnectionStatusChangedCallback );

    _listen_socket =
      _interface->CreateListenSocketIP( server_local_addr, 1, &opt );

    if ( _listen_socket == k_HSteamListenSocket_Invalid )
      printf( "Error: failed to listen on port 1234\n" );

    _poll_grp = _interface->CreatePollGroup();

    if ( _poll_grp == k_HSteamNetPollGroup_Invalid )
      printf( "Error: failed to listen on port 1234\n" );

    printf( "Server listening on port 1234...\n" );

    while ( !_quit ) {
      PollIncomingMessages();
      PollConnectionStateChanges();
      PollLocalUserInput();
      std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }

    printf( "Closing connections...\n" );

    for ( auto it: clients ) {
      // Send them one more goodbye message.  Note that we also have the
      // connection close reason as a place to send final data.  However,
      // that's usually best left for more diagnostic/debug text not actual
      // protocol strings.
      SendStringToClient( it.first, "Server is shutting down.  Goodbye." );

      // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
      // to flush this out and close gracefully.
      _interface->CloseConnection( it.first, 0, "Server Shutdown", true );
    }
  }

  void PollIncomingMessages() {}
  void PollConnectionStateChanges() {}
  void PollLocalUserInput() {}

  void SendStringToClient( HSteamNetConnection conn, const char *str ) {
    _interface->SendMessageToConnection(
      conn,
      str,
      (uint32) strlen( str ),
      k_nSteamNetworkingSend_Unreliable,
      nullptr );
  }
};

int main() {
  printf( "In server.cpp!\n" );

  return 0;
}
