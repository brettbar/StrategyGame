#pragma once

#include "network.hpp"
#include <isteammatchmaking.h>

namespace Network {


  class IHost {
public:
    static IHost *Host() {
      static IHost instance;
      return &instance;
    }

    void Init() {
      SteamAPICall_t steam_api_call =
        SteamMatchmaking()->CreateLobby( k_ELobbyTypePublic, 2 );

      result_lobby_created.Set( steam_api_call, this, &IHost::OnLobbyCreated );

      Network::is_host = true;
    }

    void CheckForMessages() {
      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
        if ( !_clients[i].active )
          continue;

        Network::CheckForMessages( _clients[i].conn );
      }
    }

    void Delete() {
      delete this;
    }

    std::vector<ClientConnectionData> GetConnectedUsers() {
      std::vector<ClientConnectionData> clients = {};

      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; i++ ) {
        if ( _clients[i].active ) {
          clients.push_back( _clients[i] );
        }
      }

      return clients;
    }

private:
    IHost( IHost const & ) = delete;
    void operator=( const IHost & ) = delete;

    // CSteamID _lobby_id;
    CSteamID _server_id;


    // Used after lobby is done
    ClientConnectionData _clients[MAX_PLAYERS_PER_SERVER];

    HSteamListenSocket _socket;
    // HSteamNetPollGroup _poll_grp;

    void OnLobbyCreated( LobbyCreated_t *, bool );
    CCallResult<IHost, LobbyCreated_t> result_lobby_created;

    STEAM_CALLBACK( IHost, OnLobbyChatMsg, LobbyChatMsg_t );

    STEAM_CALLBACK(
      IHost,
      OnNetConnectionStatusChanged,
      SteamNetConnectionStatusChangedCallback_t
    );

    IHost() {
      // zero the client connection data
      memset( &_clients, 0, sizeof( _clients ) );


      // _poll_grp = SteamNetworkingSockets()->CreatePollGroup();

      // make this if debug
      if ( LOCAL )
        SetupUsingIP();
      else
        SetupUsingP2P();
    }

    ~IHost() {
      // Close connection
      printf( "~IHost()\n" );

      SteamNetworkingSockets()->CloseListenSocket( _socket );
      // SteamNetworkingSockets()->DestroyPollGroup( poll_grp );
    }


    // This is useful for localhost testing on the same machine
    void SetupUsingIP() {
      SteamNetworkingIPAddr addr = SteamNetworkingIPAddr();
      addr.SetIPv6LocalHost( 10202 );

      printf( "Hosting with addr: %d\n", addr.GetIPv4() );
      _socket =
        SteamNetworkingSockets()->CreateListenSocketIP( addr, 0, nullptr );
      assert( _socket != k_HSteamListenSocket_Invalid );
    }

    // This is useful for real production, different machines
    void SetupUsingP2P() {
      _socket =
        SteamNetworkingSockets()->CreateListenSocketP2P( 0, 0, nullptr );
      assert( _socket != k_HSteamListenSocket_Invalid );
    }


    void SendMessageToAllPeers( const char *msg ) {

      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
        if ( !_clients[i].active )
          continue;

        SendMessageToPeer( _clients[i].conn, msg );
      }
    }
  };

  inline IHost *Host() {
    return IHost::Host();
  }


  inline void IHost::OnLobbyCreated( LobbyCreated_t *cb, bool io_failure ) {
    if ( cb->m_eResult == k_EResultOK ) {
      printf( "Lobby created successfully\n" );

      Network::lobby_id = cb->m_ulSteamIDLobby;

      char rgchLobbyName[256];
#if _WINDLL
      sprintf_s(
        rgchLobbyName, "%s's lobby", SteamFriends()->GetPersonaName()
      );
#else
      snprintf(
        rgchLobbyName,
        sizeof( rgchLobbyName ),
        "%s's lobby",
        SteamFriends()->GetPersonaName()
      );
#endif

      SteamMatchmaking()->SetLobbyData(
        Network::lobby_id, "name", rgchLobbyName
      );
      printf( "Hosting lobby %s\n", rgchLobbyName );

      // TODO Move to after a player is found
    }
    else {
      printf( "Failed to create lobby, lost connection to steam back-end\n" );
    }
  }

  inline void IHost::OnLobbyChatMsg( LobbyChatMsg_t *param ) {
    char buf[4 * 1024];
    SteamMatchmaking()->GetLobbyChatEntry(
      param->m_ulSteamIDLobby,
      param->m_iChatID,
      nullptr,
      buf,
      sizeof( buf ),
      NULL
    );

    printf( "> %s\n", buf );
  }

  inline void IHost::OnNetConnectionStatusChanged(
    SteamNetConnectionStatusChangedCallback_t *cb
  ) {
    SteamNetConnectionInfo_t info = cb->m_info;
    ESteamNetworkingConnectionState old_state = cb->m_eOldState;

    if ( _socket == k_HSteamListenSocket_Invalid ) {
      printf( "Invalid socket, bailing\n" );
      return;
    }

    if ( info.m_hListenSocket != _socket ) {
      printf( "Socket doesn't match?, bailing\n" );
      return;
    }


    if ( info.m_eState == k_ESteamNetworkingConnectionState_Connecting && old_state == k_ESteamNetworkingConnectionState_None ) {
      printf(
        "Accepting client connection %s\n", info.m_szConnectionDescription
      );

      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
        if ( !_clients[i].active ) {
          EResult res =
            SteamNetworkingSockets()->AcceptConnection( cb->m_hConn );

          if ( res != k_EResultOK ) {
            char msg[256];
#if _WINDLL
            sprintf_s( msg, "AcceptConnection returned %d", res );
#else
            snprintf( msg, sizeof( msg ), "AcceptConnection returned %d", res );
#endif


            printf( "%s\n", msg );

            SteamNetworkingSockets()->CloseConnection(
              cb->m_hConn,
              k_ESteamNetConnectionEnd_AppException_Generic,
              "Failed to accept connection",
              false
            );
            return;
          }

          _clients[i].steam_user_id = info.m_identityRemote.GetSteamID();
          _clients[i].active = true;
          _clients[i].conn = cb->m_hConn;

          SendMessageToPeer(
            cb->m_hConn, "IHost >> Hey Client, I'll let you in"
          );

          SendMessageToAllPeers( "New client connected" );


          // TODO make this only close when the game is started
          SteamMatchmaking()->LeaveLobby( lobby_id );

          return;
        }
      }

      printf( "Rejecting connection; server full\n" );
      SteamNetworkingSockets()->CloseConnection(
        cb->m_hConn,
        k_ESteamNetConnectionEnd_AppException_Generic,
        "Server full!",
        false
      );
    }
    // check if client has disconnected
    else if ( ( old_state == k_ESteamNetworkingConnectionState_Connecting || old_state == k_ESteamNetworkingConnectionState_Connected ) && info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer ) {
      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
        if ( !_clients[i].active )
          continue;

        if ( _clients[i].steam_user_id == info.m_identityRemote.GetSteamID() ) {
          printf( "Disconnected dropped user\n" );

          SteamNetworkingSockets()->CloseConnection(
            _clients[i].conn,
            k_ESteamNetConnectionEnd_App_Min + 1,
            nullptr,
            false
          );
        }
      }
    }
  }

};// namespace Network
