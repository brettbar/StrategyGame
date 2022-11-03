
#include "../common.hpp"
#include "../state.hpp"
#include "../systems/animation_system.hpp"
#include "../systems/map/map_system.hpp"
#include "../systems/overlay_system.hpp"
#include "../systems/selection_system.hpp"
#include "../systems/settlement_system.hpp"
#include "fonts.hpp"
#include "textures.hpp"

#include <raylib.h>

namespace Renderer {

inline Shader shader;
inline Shader outline_shader;

inline void DrawActors( bool debug );

inline void Init( State &state ) {

  shader = LoadShader( "assets/shaders/pixel.vs", "assets/shaders/pixel.fs" );
  // LoadShader( 0, 0 );
  outline_shader = LoadShader( 0, "assets/shaders/outline.fs" );

  i32 outline_size_loc = GetShaderLocation( outline_shader, "outlineSize" );
  i32 outline_color_loc = GetShaderLocation( outline_shader, "outlineColor" );

  float outline_size = 4.0f;
  float outline_color[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

  SetShaderValue(
    outline_shader,
    outline_size_loc,
    &outline_size,
    SHADER_UNIFORM_FLOAT );
  SetShaderValue(
    outline_shader,
    outline_color_loc,
    outline_color,
    SHADER_UNIFORM_VEC4 );


  state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( state.mapWidth * 64.0f ) / 2, ( state.mapHeight * 64.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };
}


inline void Draw( State &state, TextureCache &texture_cache ) {
  ClearBackground( DARKGRAY );

  BeginMode2D( state.camera );

  BeginBlendMode( BLEND_ALPHA_PREMULTIPLY );
  {

    BeginShaderMode( shader );
    {
      // Draw Terrain
      Terrain::Draw( state.camera, texture_cache );
    }
    EndShaderMode();

    // Overlay shouldn't be ran through shader?
    OverlaySystem::DrawProvinceOverlays( texture_cache );
    SelectionSystem::Draw(
      texture_cache,
      state.gameState == GameState::EDITOR );
    OverlaySystem::DrawSettlementOverlays( texture_cache );


    BeginShaderMode( shader );
    {
      switch ( MapSystem::mode ) {
        case MapSystem::Mode::TERRAIN: {
          SettlementSystem::Draw( texture_cache, false );

        } break;
        case MapSystem::Mode::POLITICAL: {
          SettlementSystem::Draw( texture_cache, false );
        } break;
      }
    }
    EndShaderMode();

    // AnimationSystem::Draw( reg, state.gameState == GameState::EDITOR );
    DrawActors( state.gameState == GameState::EDITOR );
  }
  EndBlendMode();


  EndMode2D();
}

inline void DrawActors( bool debug ) {
  entt::basic_view actors =
    Global::registry.view<Unit::Component, Animated::Component>();

  Global::registry.sort<Unit::Component>(
    []( const Unit::Component &lhs, const Unit::Component &rhs ) {
      return rhs.position.y > lhs.position.y;
    } );

  actors.each( [debug]( Unit::Component &unit, Animated::Component &anim ) {
    //    DrawTextureV(
    //        unit.sprite,
    //        {unit.position.x - 64.0f, unit.position.y - 64.0f},
    //        WHITE);


    if ( unit.selected ) {

      BeginShaderMode( outline_shader );
      DrawPerfectTexture(
        anim.sprite,
        anim.frameRec,
        { unit.position.x - 64.0f, unit.position.y - 64.0f },
        WHITE );
      EndShaderMode();
    } else {
      BeginShaderMode( shader );
      DrawPerfectTexture(
        anim.sprite,
        anim.frameRec,
        { unit.position.x - 64.0f, unit.position.y - 64.0f },
        WHITE );
      EndShaderMode();
    }

    // DrawPerfectTexture(
    //   anim.sprite,
    //   anim.frameRec,
    //   { unit.position.x - 64.0f, unit.position.y - 64.0f },
    //   WHITE );


    // DrawTextureRec(
    //   anim.sprite,
    //   anim.frameRec,
    //   { unit.position.x - 64.0f, unit.position.y - 64.0f },
    //   WHITE );

    if ( debug && Vector2Distance( unit.position, unit.destination ) > 0.5f ) {
      DrawLineEx( unit.position, unit.destination, 2, MAGENTA );
    }
  } );
}


};// namespace Renderer
