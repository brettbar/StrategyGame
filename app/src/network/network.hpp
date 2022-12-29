#include <assert.h>

#include <stdio.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingtypes.h>


namespace Network {

class Host {
  int virtual_port_local = 0;// Used when listening and when connecting

  SteamNetworkingIdentity identity_local;
  const char *signal_service_addr;

  HSteamListenSocket listen_socket;
  HSteamNetConnection net_connection;


  public:
  Host( const char *addr ) {
    identity_local.Clear();
    signal_service_addr = addr;
  }

  void Listen() {
    printf(
      "Identity %s is starting server",
      SteamNetworkingIdentityRender( identity_local ).c_str()
    );
    listen_socket = SteamNetworkingSockets()->CreateListenSocketP2P(
      virtual_port_local, 0, nullptr
    );
    assert( listen_socket != k_HSteamListenSocket_Invalid );
  }
};

class Client {
  SteamNetworkingIdentity identity_remote;
  int virtual_port_remote = 0;// Only used when connecting


  public:
  Client() {
    identity_remote.Clear();
  }

  void Connect() {
    printf(
      "Identity %s is attempting to connect to server",
      SteamNetworkingIdentityRender( identity_remote ).c_str()
    );
  }
};


};// namespace Network
