#include "steam/isteammatchmaking.h"
#include <steam/steam_api.h>


#include <assert.h>

#include <stdarg.h>
#include <stdio.h>


#include <iostream>


namespace Network {

inline CSteamID lobby_id;


// typedef void (*FSteamNetworkingSocketsDebugOutput)( ESteamNetworkingSocketsDebugOutputType nType, const char *pszMsg );

struct Host {

  inline void Setup() {

    SteamMatchmaking()->CreateLobby( k_ELobbyTypePublic, 2 );
    LobbyCreated_t lobby_data = LobbyCreated_t();

    lobby_id = lobby_data.m_ulSteamIDLobby;

    SteamMatchmaking()->SetLobbyJoinable( lobby_data.m_ulSteamIDLobby, true );
  }
};

struct Client {
  inline void Setup() {
    SteamMatchmaking()->JoinLobby( lobby_id );
  }
};

};// namespace Network
