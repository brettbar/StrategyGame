#pragma once

#include "../shared/common.hpp"

#include "../world/managers/map_manager.hpp"

#include "../world/components/animated_component.hpp"

#include "../world/systems/settlement_system.hpp"
#include "../world/systems/overlay_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/province_system.hpp"

#include <raylib.h>

namespace Renderer {

  inline Shader shader;
  inline Shader outline_shader;

  inline void DrawActors( bool debug );
  inline void draw_map( Map::Mode );


  inline void Init() {
    shader = LoadShader(
      "assets/shaders/pixel.vs", "assets/shaders/pixel.fs"
    );
    // LoadShader( 0, 0 );
    outline_shader = LoadShader( 0, "assets/shaders/outline.fs" );

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
  }


  inline void draw_map( Map::Mode map_mode ) {
    ClearBackground( DARKGRAY );

    BeginMode2D( Global::state.camera );

    BeginBlendMode( BLEND_ALPHA_PREMULTIPLY );
    // BeginBlendMode( BLEND_ALPHA );
    {
      // First, always draw terrain
      BeginShaderMode( shader );
      {
        // Draw Terrain
        ProvinceSystem::Draw( Global::state.camera );
      }
      EndShaderMode();
    }
    EndBlendMode();

    switch ( map_mode ) {
      case Map::Mode::Default:
        // Draw TransparentOverlay

        OverlaySystem::draw_default();
        BeginShaderMode( shader );
        {
          OverlaySystem::draw_borders();
          Selection::Draw( Global::texture_cache, true );
        }
        EndShaderMode();

        OverlaySystem::draw_settlement_name();
        break;
      case Map::Mode::Terrain:
        // Do nothing
        break;
      case Map::Mode::Political:
        // Draw OpaqueOverlay

        OverlaySystem::draw_political();
        BeginShaderMode( shader );
        {
          OverlaySystem::draw_borders();
          Selection::Draw( Global::texture_cache, true );
        }
        OverlaySystem::draw_settlement_name();
        EndShaderMode();
        break;
      case Map::Mode::Resources:
        // Draw Resources
        BeginShaderMode( shader );
        {
          ResourceSystem::Draw( Global::state.camera );
        }
        EndShaderMode();
        break;
    }


    BeginBlendMode( BLEND_ALPHA_PREMULTIPLY );
    // BeginBlendMode( BLEND_ALPHA );
    {
      BeginShaderMode( shader );
      {
        switch ( map_mode ) {
          case Map::Mode::Default:
            Settlement::System::draw( Global::texture_cache, false );
            break;
          case Map::Mode::Terrain:
            Settlement::System::draw( Global::texture_cache, false );
            break;
          case Map::Mode::Political:
            Settlement::System::draw( Global::texture_cache, false );
            break;
          case Map::Mode::Resources:
            Settlement::System::draw( Global::texture_cache, false );
            break;
        }

        // AnimationSystem::Draw( reg, state.gameState == GameState::EDITOR );
        DrawActors(
          // Global::state.gameState == GameState::EDITOR
          true
        );
      }
      EndShaderMode();
    }
    EndBlendMode();


    EndMode2D();
  }

  inline void DrawUI() {
    // TODO right now alpha issues are cropping up with the shader
    // BeginShaderMode( shader );

    // UI::System::Draw();
    // EndShaderMode();
  }

  inline void DrawActors( bool debug ) {
    entt::basic_view actors =
      Global::world.view<Actor::Component, Animated::Component>();

    Global::world.sort<Actor::Component>(
      []( const Actor::Component &lhs, const Actor::Component &rhs ) {
        return rhs.position.y > lhs.position.y;
      }
    );

    actors.each( [debug]( Actor::Component &actor, Animated::Component &anim ) {
      //    DrawTextureV(
      //        actor.sprite,
      //        {actor.position.x - 64.0f, actor.position.y - 64.0f},
      //        WHITE);


      if ( actor.selected ) {
        Texture2D texture = Global::texture_cache[hstr{anim.sprite_id.c_str()}]->texture;

        BeginShaderMode( outline_shader );
        {
          DrawTextureRec(
            texture,
            anim.frameRec,
            { actor.position.x - 64.0f, actor.position.y - 64.0f },
            WHITE
          );
        }
        EndShaderMode();
      } else {
        Texture2D texture = Global::texture_cache[hstr{anim.sprite_id.c_str()}]->texture;

        BeginShaderMode( shader );
        {
          DrawTextureRec(
            texture,
            anim.frameRec,
            { actor.position.x - 64.0f, actor.position.y - 64.0f },
            WHITE
          );
        }
        EndShaderMode();
      }

      // DrawPerfectTexture(
      //   anim.sprite,
      //   anim.frameRec,
      //   { actor.position.x - 64.0f, actor.position.y - 64.0f },
      //   WHITE );


      // DrawTextureRec(
      //   anim.sprite,
      //   anim.frameRec,
      //   { actor.position.x - 64.0f, actor.position.y - 64.0f },
      //   WHITE );

      if ( debug && Vector2Distance( actor.position, actor.destination ) > 0.5f ) {
        DrawLineEx( actor.position, actor.destination, 2, MAGENTA );
      }
    } );
  }


};// namespace Renderer
