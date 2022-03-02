#pragma once

#include "GameNetworkingSockets/steam/isteamnetworkingsockets.h"

HSteamListenSocket g_hListenSocket;
HSteamNetConnection g_hConnection;


int main()
{
  SteamNetworkingIdentity identityLocal;
  identityLocal.Clear();
  SteamNetworkingIdentity identityRemote;
  identityRemote.Clear();
}
