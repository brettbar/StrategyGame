#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../../shared/utils.hpp"
#include "../components/actor.hpp"
#include "../components/player.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/settlement.hpp"

#include "../../signals/updates.hpp"


namespace SelectionSystem {

  template<typename T>
  inline void ClearSelection( view<T> component_view ) {
    for ( entt::entity entity: component_view ) {
      T &component = Global::world.get<T>( entity );
      component.selected = false;
      Global::world.remove<Selected::Component>( entity );
    }
  }

  inline entt::entity GetSelectedEntity() {
    return Global::world.view<Selected::Component>().front();
  }
  template<typename... T>
  inline bool Selected() {
    auto selected_entity = Global::world.view<Selected::Component>().front();
    return Global::world.all_of<T...>( selected_entity );
  }

  inline void Draw( TextureCache &cache, bool isDebug ) {
    auto provsView =
      Global::world.view<Province::Component, Selected::Component>();

    for ( auto entity: provsView ) {
      auto &prov = provsView.get<Province::Component>( entity );

      if ( isDebug ) {
        DrawTexture(
          cache[hstr{ "tile_outline" }]->texture,
          prov.tile->position.x,
          prov.tile->position.y,
          WHITE
        );
      }
    }
  }


  inline void CheckSelectActors( vec2f click_pos, std::string player_id ) {
    auto actors_view = Global::world.view<Actor::Component>();
    auto player_view = Global::world.view<Player::Component>();
    auto selected_entity = Global::world.view<Selected::Component>().front();

    entt::entity owner_e = entt::null;

    for ( auto player_e: player_view ) {
      Player::Component player_c =
        player_view.get<Player::Component>( player_e );

      if ( player_id == player_c.player_id ) {
        owner_e = player_e;
      }
    }

    // use forward iterators and get only the components of interest
    for ( auto &entity: actors_view ) {
      if ( selected_entity != entt::null )
        return;

      Actor::Component &actor = actors_view.get<Actor::Component>( entity );

      if ( actor.owner != owner_e )
        continue;

      if ( CheckCollisionPointCircle( actor.position, click_pos, 32 ) ) {
        Global::world.emplace<Selected::Component>( entity, player_id );

        // std::cout << EntityIdToString( entity ) << std::endl;
        // std::cout << EntityIdToString( actor.owner ) << std::endl;

        actor.selected = true;
        selected_entity = entity;

        InterfaceUpdate::Update{
          .id = InterfaceUpdate::ID::ActorContext,
          .condition = true,
        }
          .Send();

        return;
      }
    }
  }

  template<typename T>
  inline T *GetSelectedComponent() {
    auto selected_entity = GetSelectedEntity();

    if ( Global::world.all_of<T>( selected_entity ) ) {
      return &Global::world.get<T>( selected_entity );
    }

    return nullptr;
  }

  inline void CheckSelectProvince( vec2f click_pos, std::string player_id ) {
    i32 tile_pos_id = DetermineTileIdFromPosition( click_pos );
    auto prov_view = Global::world.view<Province::Component>();
    auto selected_entity = Global::world.view<Selected::Component>().front();

    if ( tile_pos_id == -1 )
      return;

    for ( auto &entity: prov_view ) {
      if ( selected_entity != entt::null )
        return;

      auto &prov = prov_view.get<Province::Component>( entity );

      if ( tile_pos_id == prov.tile->id ) {
        Global::world.emplace<Selected::Component>( entity, player_id );

        std::cout << EntityIdToString( entity ) << std::endl;
        std::cout << EntityIdToString( prov.tile->owner ) << std::endl;

        selected_entity = entity;

        if ( Global::world.all_of<Settlement::Component>( selected_entity ) ) {
          Settlement::Component settlement =
            Global::world.get<Settlement::Component>( selected_entity );

          InterfaceUpdate::Update{
            .id = InterfaceUpdate::ID::SettlementContext,
            .condition = true,
            .json =
              {
                { "name", settlement.name },
                { "population",
                  std::to_string( settlement.population.current ) },
                { "development",
                  Settlement::dev_names.at( settlement.development ) },
              },
          }
            .Send();
        } else {
          InterfaceUpdate::Update{
            .id = InterfaceUpdate::ID::SettlementContext,
            .condition = true,
            .json =
              {
                { "name", "Uninhabited" },
                { "population", "Uninhabited" },
                { "development", "Uninhabited" },
              },
          }
            .Send();
        }

        return;
      }
    }
  }

  inline void UpdateSelection( Vector2 click_pos, std::string player_id ) {
    view<Actor::Component> actors_view = Global::world.view<Actor::Component>();
    view<Province::Component> prov_view =
      Global::world.view<Province::Component>();


    ClearSelection<Actor::Component>( actors_view );
    ClearSelection<Province::Component>( prov_view );

    InterfaceUpdate::Update{
      .id = InterfaceUpdate::ID::SettlementContext,
      .condition = false,
    }
      .Send();
    InterfaceUpdate::Update{
      .id = InterfaceUpdate::ID::ActorContext,
      .condition = false,
    }
      .Send();

    CheckSelectActors( click_pos, player_id );
    CheckSelectProvince( click_pos, player_id );
  }

};// namespace SelectionSystem
