
#pragma once

#include "../../shared/global.hpp"

#include "component.hpp"

namespace fs = std::filesystem;

namespace Actor {
  class Manager {
    Manager() {}
    ~Manager() {}
    Manager( Manager const & ) = delete;

    void operator=( const Manager & ) = delete;

public:
    std::map<str, Actor::Data> actor_data = {};
    static Manager *Get() {
      static Manager instance;
      return &instance;
    }


    void load_actors() {
      std::ifstream f( "src/data/actors.json" );
      {
        nlohmann::json js = nlohmann::json::parse( f );

        for ( auto &faction: js.items() ) {
          for ( auto &actor: faction.value().items() ) {

            str type = actor.value().at( "type" );
            str faction_id = faction.key();
            str actor_id = faction_id + "_" + type;
            str name = actor.value().at( "name" );
            str sprite_id = ( faction_id + "_" + name ).c_str();

            actor_data.emplace(
              actor_id,
              Actor::Data{
                Actor::type_lookup.at( type ),
                actor_id,
                name,
                faction_id,
                sprite_id,
              }
            );
          }
        }
      }
      f.close();


      str root = "src/assets/images/actors";

      // for each faction
      for ( const auto &faction_folder: fs::directory_iterator( root ) ) {
        std::string faction = faction_folder.path().filename().generic_string();


        // for each file in the folder
        for ( const auto &actor_file:
              fs::directory_iterator( faction_folder ) ) {

          str path = actor_file.path().generic_string();
          str sprite =
            faction + "_" + actor_file.path().stem().generic_string();

          std::cout << "Adding to texture cache: " << sprite << "\n";

          LoadAsset(
            hstr{ sprite.c_str() },
            LoadImage( path.c_str() ),
            Global::texture_cache
          );
        }
      }

      // LoadAsset(
      //   hstr{ "romans_villager_texture" },
      //   LoadImage( ( path + "/romans/actors/roman_villager.png" ).c_str() ),
      //   Global::texture_cache
      // );
      LoadTexturePointFilter(
        hstr{ "romans_colonist_overview" },
        CropUnitImage( ( root + "/romans/plebeian.png" ).c_str() ),
        Global::texture_cache
      );
      // LoadAsset(
      //   hstr{ "romans_hastati_texture" },
      //   LoadImage( ( path + "/romans/actors/roman_hastati.png" ).c_str() ),
      //   Global::texture_cache
      // );
      LoadTexturePointFilter(
        hstr{ "romans_hastati_overview" },
        CropUnitImage( ( root + "/romans/hastati.png" ).c_str() ),
        Global::texture_cache
      );

      // LoadAsset(
      //   hstr{ "greeks_villager_texture" },
      //   LoadImage( ( path + "/greeks/actors/greek_villager.png" ).c_str() ),
      //   Global::texture_cache
      // );

      // LoadAsset(
      //   hstr{ "celts_villager_texture" },
      //   LoadImage( ( path + "/celts/actors/celtic_villager.png" ).c_str() ),
      //   Global::texture_cache
      // );
    }
  };
}// namespace Actor