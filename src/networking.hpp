#pragma once

#include "GameNetworkingSockets/steam/isteamnetworkingsockets.h"


class Networking : ISteamNetworkingSockets
{
  HSteamNetConnection ConnectByIPAddress(
    const SteamNetworkingIPAddr &address,
    int nOptions,
    const SteamNetworkingConfigValue_t *pOptions ) override = 0;
};
