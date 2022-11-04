#pragma once

#include "../components/ai.hpp"
#include "../components/player.hpp"
#include "../data/factions.hpp"
#include "../global.hpp"

namespace PlayerSystem {

inline void Init();
inline void Update();
inline void HumanUpdate( Player::Component & );
inline void AIUpdate( Player::Component & );

inline void Init() {
  for ( u32 i = 0; i < 7; i++ ) {
    entt::entity new_player = Global::world.create();
    if ( i == 0 )
      Global::world.emplace<Player::Component>(
        new_player,
        true,
        Faction::ID::Romans );
    else {
      Global::world.emplace<Player::Component>(
        new_player,
        true,
        (Faction::ID) i );
      Global::world.emplace<AI::Component>( new_player );
    }
  }
}

inline void Update() {
  for ( auto entity: Global::world.view<Player::Component>() ) {
    Player::Component &player = Global::world.get<Player::Component>( entity );

    if ( player.is_human ) {
      HumanUpdate( player );
    } else {
      AIUpdate( player );
    }
  }
}

inline void HumanUpdate( Player::Component &player ) {}

inline void AIUpdate( Player::Component &player ) {}

};// namespace PlayerSystem

// class TempPS {
//   public:
//   u32 id;
//   Faction faction;
//   std::map<const char *, hstr> textureMap;

//   TempPS( u32 id, Faction faction ) {
//     this->id = id;
//     this->faction = faction;
//     RefreshTextureMap();
//   }
//   ~TempPS() {}

//   void RefreshTextureMap() {
//     switch ( faction ) {
//       case Faction::ROMANS:
//         this->textureMap = {
//           {
//             "Villager",
//             hstr{ "romanVillagerTexture" },
//           },
//         };
//         break;
//       case Faction::GREEKS:
//         this->textureMap = {
//           {
//             "Villager",
//             hstr{ "greekVillagerTexture" },
//           },
//         };
//         break;
//       case Faction::CELTS:
//         this->textureMap = {
//           {
//             "Villager",
//             hstr{ "celtVillagerTexture" },
//           },
//         };
//         break;
//       case Faction::PUNICS:
//         this->textureMap = {
//           {
//             "Villager",
//             hstr{ "punicVillagerTexture" },
//           },
//         };
//         break;
//       case Faction::PERSIANS:
//         this->textureMap = {
//           {
//             "Villager",
//             hstr{ "persianVillagerTexture" },
//           },
//         };
//         break;
//       default:
//         break;
//     }
//   }
// };
