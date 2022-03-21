#include "factions.h"

namespace Factions {

// void InitPlayers(entt::registry &registry, u32 numPlayers) {
//   if (numPlayers > MAX_PLAYERS)
//     numPlayers = MAX_PLAYERS;

//   Civilization civs[MAX_PLAYERS] = {
//       Roman_Republic, Greek_Cities,   Celtic_Tribes,
//       Punic_Colonies, Persian_Empire, Germanic_Federation,
//   };

//   //@TODO define actual color pallete
//   Color primaryColors[MAX_PLAYERS] = {
//       RED, SKYBLUE, GREEN, ORANGE, WHITE,
//   };

//   //@TODO define actual color pallete
//   Color secondaryColors[MAX_PLAYERS] = {GOLD, GOLD, RED, WHITE, PURPLE};

//   for (u32 i = 0; i < numPlayers; i++) {
//     auto entity = registry.create();
//     registry.emplace<Faction>(entity, i, civs[i], primaryColors[i],
//                               secondaryColors[i]);
//   }
// }

}; // namespace Factions
