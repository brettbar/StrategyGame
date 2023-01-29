/*
  0. Clicked Host Game
  1. Host() :: Host singleton is Created, Networking is setup using IP or P2P
  2. Init() :: Lobby is Created, MessageQueue begins processing messages
  3. OnLobbyCreated() :: Lobby is created and people can now connect
  4. Client attempts to connect, triggering SteamNetConnectionStatusChangedCallback_t,
  5. OnNetConnectionStatusChanged() :: connection is checks and accepts client connection, giving slot in _clients
  6. Once a client joins, all 
 */

#pragma once

#include "network.hpp"
#include <chrono>
#include <isteammatchmaking.h>

namespace Network {

  namespace {
    enum class MessageType {
      Lobby,
    };
    struct Message {
      MessageType type;
      std::string content;
    };
    struct MessageQueue : entt::dispatcher {
      void Enqueue( const Message &msg ) {
        this->enqueue( msg );
      }
    };
  };// namespace


  class IHost {
private:
    IHost( IHost const & ) = delete;
    void operator=( const IHost & ) = delete;

    CSteamID _server_id;


    // Used after lobby is done
    // ClientConnectionData _clients[MAX_PLAYERS_PER_SERVER];
    std::array<ClientConnectionData, MAX_PLAYERS_PER_SERVER> _clients;

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
      // memset( &_clients, 0, sizeof( _clients ) );
      _clients = {};


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

    void SendPing() {
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; i++ ) {
        if ( !_clients[i].active )
          continue;

        const auto now = std::chrono::system_clock::now();
        SendMessageOnConnection( _clients[i].conn, "ping" );
      }
    }

    void SendMessageToAllClients( const char *msg ) {

      // We start at 1 since host is always i=0
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; i++ ) {
        if ( !_clients[i].active )
          continue;

        SendMessageOnConnection( _clients[i].conn, msg );
      }
    }

public:
    MessageQueue msg_queue;

    static IHost *Host() {
      static IHost instance;
      return &instance;
    }

    void Init() {
      SteamAPICall_t steam_api_call = SteamMatchmaking()->CreateLobby(
        k_ELobbyTypePublic, MAX_PLAYERS_PER_SERVER
      );

      result_lobby_created.Set( steam_api_call, this, &IHost::OnLobbyCreated );

      msg_queue = {};

      msg_queue.sink<Message>().connect<&IHost::ProcessMessage>( this );

      Network::is_host = true;
    }

    void CheckForMessages() {
      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
        if ( !_clients[i].active )
          continue;

        CheckForMessage( _clients[i].conn );
      }
    }

    void CheckForMessage( HSteamNetConnection conn ) {
      if ( conn != k_HSteamNetConnection_Invalid ) {
        SteamNetworkingMessage_t *msg;
        int r = SteamNetworkingSockets()->ReceiveMessagesOnConnection(
          conn, &msg, 1
        );

        assert( r == 0 || r == 1 );

        if ( r == 1 ) {
          printf( "Received message: '%s'\n", (char *) msg->GetData() );

          msg_queue.Enqueue( Message{
            MessageType::Lobby,
            std::string( (char *) msg->GetData() ),
          } );

          msg->Release();
        }
      }
    }

    void EvaluateMessages() {
      msg_queue.update();
    }

    void ProcessMessage( const Message &msg ) {
      switch ( msg.type ) {
        case MessageType::Lobby: {
          printf( "Processed message: '%s'\n", msg.content.c_str() );

          return;
        }
      }
    }

    void Delete() {
      delete this;
    }


    std::vector<std::string> GetConnectedUsers() {
      std::vector<std::string> clients = {};

      for ( u32 i = 0; i < MAX_PLAYERS_PER_SERVER; i++ ) {
        if ( _clients[i].active ) {

          // if ( i > 0 ) {
          // std::cout << "player_id :: " << _clients[i].player_id << std::endl;
          // }

          std::string text =
            std::string( _clients[i].player_id ) + ": " +
            SteamFriends()->GetFriendPersonaName( _clients[i].steam_user_id );

          clients.push_back( text );
        }
      }

      return clients;
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

      _clients[0].player_id = "player_0";
      _clients[0].active = true;
      _clients[0].steam_user_id =
        SteamMatchmaking()->GetLobbyOwner( Network::lobby_id );


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

    printf( "Lobby: %s\n", buf );
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

      for ( int i = 0; i < MAX_PLAYERS_PER_SERVER; ++i ) {
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

          // printf( "Annoying number: %s\n", std::to_string( i ).c_str() );

          // char player_id[128];
          // sprintf( player_id, "player_%d", i );
          // printf( "buf result: %s\n", player_id );

          _clients[i].player_id = "player_" + std::to_string( i );
          _clients[i].steam_user_id = info.m_identityRemote.GetSteamID();
          _clients[i].active = true;
          _clients[i].conn = cb->m_hConn;

          // SendMessageOnConnection(
          //   cb->m_hConn, "IHost >> Hey Client, I'll let you in"
          // );

          // Tell the new client about all the current clients
          for ( auto client: _clients ) {
            if ( !client.active )
              continue;


            nlohmann::json host_payload = {
              { "type", "player_connected" },
            };
            host_payload["data"] = {
              { "is_host", ( client.player_id == "player_0" ) },
              { "player_id", client.player_id },
              {
                "steam_name",
                std::string(
                  SteamFriends()->GetFriendPersonaName( client.steam_user_id )
                ),
              } };

            SendMessageOnConnection(
              _clients[i].conn, host_payload.dump().c_str()
            );
          }


          // Tell all the current clietns about the new client
          nlohmann::json payload = {
            { "type", "player_connected" },
          };
          payload["data"] = {
            { "is_host", false },
            { "player_id", _clients[i].player_id },
            {
              "steam_name",
              std::string( SteamFriends()->GetFriendPersonaName(
                _clients[i].steam_user_id
              ) ),
            } };
          // SendMessageToAllClients( payload.dump().c_str() );

          // We start at 1 since the host already knows
          for ( uint32 j = 1; j < MAX_PLAYERS_PER_SERVER; j++ ) {
            if ( !_clients[i].active || i == j )// we don't need to tell ourselves
              continue;

            SendMessageOnConnection( _clients[j].conn, payload.dump().c_str() );
          }


          // TODO make this only close when the game is started
          // SteamMatchmaking()->LeaveLobby( lobby_id );

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
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; ++i ) {
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

          _clients[i].active = false;
          _clients[i].player_id = "";
          _clients[i].steam_user_id = CSteamID();
        }
      }
    }
  }

};// namespace Network
