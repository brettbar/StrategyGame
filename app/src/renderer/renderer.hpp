#include "../shared/common.hpp"
#include "../shared/fonts.hpp"
#include "../shared/textures.hpp"
#include "../ui/ui_system.hpp"
#include "../world/systems/map_system.hpp"
#include "../world/systems/overlay_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/settlement_system.hpp"

#include <raylib.h>

namespace Renderer {

inline Shader shader;
inline Shader outline_shader;

inline void DrawActors( bool debug );


inline void Init() {
  shader =
    LoadShader( "app/assets/shaders/pixel.vs", "app/assets/shaders/pixel.fs" );
  // LoadShader( 0, 0 );
  outline_shader = LoadShader( 0, "app/assets/shaders/outline.fs" );

  i32 outline_size_loc = GetShaderLocation( outline_shader, "outlineSize" );
  i32 outline_color_loc = GetShaderLocation( outline_shader, "outlineColor" );

  float outline_size = 4.0f;
  float outline_color[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

  SetShaderValue(
    outline_shader, outline_size_loc, &outline_size, SHADER_UNIFORM_FLOAT
  );
  SetShaderValue(
    outline_shader, outline_color_loc, outline_color, SHADER_UNIFORM_VEC4
  );


  Global::state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( Global::state.mapWidth * 64.0f ) / 2,
        ( Global::state.mapHeight * 64.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };
}


inline void Draw( TextureCache &texture_cache ) {


  ClearBackground( DARKGRAY );

  BeginMode2D( Global::state.camera );

  // TODO does this actually do anything?
  BeginBlendMode( BLEND_ALPHA_PREMULTIPLY );
  {

    BeginShaderMode( shader );
    {
      // Draw Terrain
      MapSystem::Draw( Global::state.camera, texture_cache );
      // TODO this is causing fps to drop to 110
      ProvinceSystem::Draw( Global::state.camera );
    }
    EndShaderMode();

    // Overlay shouldn't be ran through shader?
    OverlaySystem::DrawProvinceOverlays( texture_cache );
    SelectionSystem::Draw(
      texture_cache, Global::state.gameState == GameState::EDITOR
    );
    OverlaySystem::DrawSettlementOverlays( texture_cache );


    BeginShaderMode( shader );
    {
      switch ( MapSystem::mode ) {
        case MapSystem::Mode::Terrain: {
          SettlementSystem::Draw( texture_cache, false );
        } break;
        case MapSystem::Mode::Political: {
          SettlementSystem::Draw( texture_cache, false );
        } break;
      }

      // AnimationSystem::Draw( reg, state.gameState == GameState::EDITOR );
      DrawActors( Global::state.gameState == GameState::EDITOR );
    }
    EndShaderMode();
  }
  EndBlendMode();


  EndMode2D();
}

inline void DrawUI() {
  // TODO right now alpha issues are cropping up with the shader
  // BeginShaderMode( shader );

  UI::Draw();
  // EndShaderMode();
}

inline void DrawActors( bool debug ) {
  entt::basic_view actors =
    Global::world.view<Unit::Component, Animated::Component>();

  Global::world.sort<Unit::Component>(
    []( const Unit::Component &lhs, const Unit::Component &rhs ) {
      return rhs.position.y > lhs.position.y;
    }
  );

  actors.each( [debug]( Unit::Component &unit, Animated::Component &anim ) {
    //    DrawTextureV(
    //        unit.sprite,
    //        {unit.position.x - 64.0f, unit.position.y - 64.0f},
    //        WHITE);


    if ( unit.selected ) {
      BeginShaderMode( outline_shader );
      DrawTextureRec(
        anim.sprite,
        anim.frameRec,
        { unit.position.x - 64.0f, unit.position.y - 64.0f },
        WHITE
      );
      EndShaderMode();
    }
    else {
      BeginShaderMode( shader );
      DrawTextureRec(
        anim.sprite,
        anim.frameRec,
        { unit.position.x - 64.0f, unit.position.y - 64.0f },
        WHITE
      );
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
