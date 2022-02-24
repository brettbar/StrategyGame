#include "../../../common.hpp"
#include "../../../events.hpp"
#include "../../../resource.hpp"
#include "../../components/province.hpp"
#include "../spawn.hpp"
#include "terrain.hpp"
#include <array>
#include <raylib.h>
#include <thread>

#pragma once

namespace Provinces
{

inline void SpawnProvince( entt::registry &, u32 );

struct Listener {
  State *currState;
  entt::registry *currReg;

  void receive( const Events::Event &event )
  {
    if ( currState == nullptr || currReg == nullptr )
    {
      return;
    }

    switch ( event.type )
    {
      case Events::PROVINCES_SPAWN_PROVINCE:
        printf( "Provinces got an event!\n" );
        SpawnProvince( *currReg, currState->currPlayer->id );
        break;
      case Events::SPAWN_DELETE_SPAWNED:
        printf( "Spawn got an event!\n" );
        Spawn::DeleteSelected( *currReg );
        break;
    }
  }
};

inline Listener listener;


using ProvinceList = std::array<
  std::shared_ptr<c_Province::Province>,
  Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT>;

void DrawProvinces( entt::registry &, TextureCache &, bool );
inline void UpdatePopulation( c_Province::Settlement & );
inline void UpdateSettlement( c_Province::Settlement & );
//  void DrawSingleBorder(Province());


inline void InitProvinces( State &state, entt::registry &reg )
{
  auto tView = reg.view<Terrain::TileMap>();
  Terrain::TileMap tiles = tView.get<Terrain::TileMap>( tView.front() );

  entt::entity entity = reg.create();
  ProvinceList provinces;

  for ( u32 i = 0; i < Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT; i++ )
  {
    provinces[i] = std::make_shared<c_Province::Province>(
      c_Province::Province{ .id = i, .owner = -1, .tile = tiles[i] } );
  }


  Events::dispatcher.sink<Events::Event>().connect<&Listener::receive>(
    listener );

  reg.emplace<ProvinceList>( entity, provinces );
}


inline void UpdateProvinces( State &state, entt::registry &reg )
{
  listener = {
    .currState = &state,
    .currReg = &reg,
  };
  // Events::dispatcher.update<Events::ProvincesEvent>();

  auto view = reg.view<ProvinceList>();
  ProvinceList &provinces = view.get<ProvinceList>( view.front() );

  for ( std::shared_ptr<c_Province::Province> prov: provinces )
  {
    if ( prov->owner > -1 && prov->settlement != nullptr )
      UpdateSettlement( *prov->settlement );
  }
}

inline void UpdateSettlement( c_Province::Settlement &settlement )
{
  UpdatePopulation( settlement );
}

inline void UpdatePopulation( c_Province::Settlement &settlement )
{
  c_Province::Population &pop = settlement.population;
  // if growing exponentially
  // P(t) = P0*e^(kt)

  // dP/dt = r * P ( 1 - P/K )
  // dP/dt = r * P ( K - P ) / K
  pop.growthRate = ( pop.birthRate - pop.deathRate ) / pop.current;
  f32 dP_over_dt = pop.growthRate * pop.current *
                   ( pop.carryingCapacity - pop.current ) /
                   pop.carryingCapacity;

  settlement.population.current += (i32) dP_over_dt;
}

inline void
SetProvinceOwner( entt::registry &registry, u32 owner, Vector2 clickPos )
{
  i32 provId = DetermineTileIdFromClick( clickPos );
  assert( provId >= 0 );

  auto provincesView = registry.view<ProvinceList>();
  auto provincesEntity = provincesView.front();
  ProvinceList &provinces = provincesView.get<ProvinceList>( provincesEntity );

  for ( std::shared_ptr<c_Province::Province> prov: provinces )
  {

    if ( prov->id == (u32) provId )
    {
      if ( prov->tile->biome == c_Tile::WATER ) continue;

      prov->owner = owner;

      if ( prov->settlement == nullptr )
      {
        prov->settlement = std::make_unique<c_Province::Settlement>();
        prov->settlement->id = prov->id;
        prov->settlement->population = {
          .current = 200,
          .birthRate = 40,
          .deathRate = 10,
          .growthRate = ( 40.0f - 10.0f ) / 200,
          .carryingCapacity = 1000,
        };

        switch ( prov->owner )
        {
          case 0:
            prov->settlement->name = "Rome";
            break;
          case 1:
            prov->settlement->name = "Athens";
            break;
          case 2:
            prov->settlement->name = "Lugudunon";
            break;
          case 3:
            prov->settlement->name = "Carthage";
            break;
          case 4:
            prov->settlement->name = "Persepolis";
            break;
        }
      }
    }
  }
}

inline void SpawnProvince( entt::registry &reg, u32 owner )
{
  auto selectedView = reg.view<c_Selected::Selected, c_Unit::Unit>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null )
  {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  c_Unit::Unit &unit = selectedView.get<c_Unit::Unit>( selectedEntity );

  i32 provId = DetermineTileIdFromClick( unit.position );
  assert( provId >= 0 );

  auto provincesView = reg.view<ProvinceList>();
  auto provincesEntity = provincesView.front();
  ProvinceList &provinces = provincesView.get<ProvinceList>( provincesEntity );

  for ( std::shared_ptr<c_Province::Province> prov: provinces )
  {

    if ( prov->id == (u32) provId )
    {
      if ( prov->tile->biome == c_Tile::WATER ) continue;

      prov->owner = owner;

      if ( prov->settlement == nullptr )
      {
        prov->settlement = std::make_unique<c_Province::Settlement>();
        prov->settlement->id = prov->id;
        prov->settlement->population = {
          .current = 200,
          .birthRate = 40,
          .deathRate = 10,
          .growthRate = ( 40.0f - 10.0f ) / 200,
          .carryingCapacity = 1000,
        };

        switch ( prov->owner )
        {
          case 0:
            prov->settlement->name = "Rome";
            break;
          case 1:
            prov->settlement->name = "Athens";
            break;
          case 2:
            prov->settlement->name = "Lugudunon";
            break;
          case 3:
            prov->settlement->name = "Carthage";
            break;
          case 4:
            prov->settlement->name = "Persepolis";
            break;
        }
      }
    }
  }
}


inline void
DrawProvinces( entt::registry &reg, TextureCache &cache, bool showOverlays )
{
  auto view = reg.view<ProvinceList>();
  ProvinceList &provinces = view.get<ProvinceList>( view.front() );

  for ( std::shared_ptr<c_Province::Province> prov: provinces )
  {
    // str idString = std::to_string(tile.id);
    // const char *idText = idString.c_str();
    // if (debug)
    //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
    //   14,
    //            WHITE);

    // str ownerString = std::to_string(tile.owner);
    // const char *ownertext = ownerString.c_str();
    // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0,
    // 14,
    //          BLUE);

    //       str coordString = std::to_string((u32)tile.coord.x) + "," +
    //                         std::to_string((u32)tile.coord.y);
    //       const char *coordText = coordString.c_str();
    ////       if (.debug)
    //         DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
    //                  BLUE);

    // str popString = std::to_string(tile.population);
    // const char *text = popString.c_str();
    // if (debug)
    //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
    //   14,
    //            RED);

    if ( prov->owner > -1 )
    {
      Color color = BLACK;

      Rectangle frameRec = { 0.0, 0.0, 128, 128.0 };

      switch ( prov->owner )
      {
        case 0:
          color = RED;
          frameRec.x = 0.0f;
          break;
        case 1:
          color = SKYBLUE;
          frameRec.x = 128.0f;
          break;
        case 2:
          color = GREEN;
          frameRec.x = 256.0f;
          break;
        case 3:
          color = PURPLE;
          frameRec.x = 384.0f;
          break;
        case 4:
          color = ORANGE;
          frameRec.x = 513.0f;
          break;
      }

      if ( showOverlays )
        DrawTextureRec(
          cache.handle( hstr{ "factionOverlay" } )->texture,
          frameRec,
          prov->tile->position,
          Fade( WHITE, 0.5 ) );
    }

    if ( prov->settlement != nullptr )
    {
      if ( prov->settlement->population.current > 0 )
      {
        Vector2 provPos = prov->tile->position;

        // DrawRectangleRec({provPos.x + 50,
        //                   provPos.y + 86, 128, 64},
        //                  Fade(WHITE, 0.8f));
        // DrawSingleBorder(tile);

        DrawTextureV(
          cache.handle( hstr{ "romanVillageTexture" } )->texture,
          provPos,
          WHITE );


        DrawRectangleRec( { provPos.x + 64, provPos.y + 96, 32, 14 }, BLACK );
        // std::string popStr = "Pop: " + std::to_string(prov->settlement->population.current);

        // DrawText(popStr.c_str(), provPos.x + 50, provPos.y + 100, 10, BLACK);


        // DrawRectangleRec({provPos.x + 50, provPos.y + 86, 128, 14}, BLACK);

        if ( prov->settlement->name != "" )
          DrawText(
            prov->settlement->name.c_str(),
            provPos.x + 64.0,
            provPos.y + 86.0,
            14,
            WHITE );
      }
    }
  }
}


//  Tile *FindTileByCoord(TileMap &tileMap, u32 x, u32 y)
//  {
//
//    if ((x >= 0 && x <= MAP_WIDTH) && (y >= 0 && y <= MAP_HEIGHT))
//    {
//      return tileMap.tiles[index(x, y)];
//    }
//    return nullptr;
//  }
};// namespace Provinces
