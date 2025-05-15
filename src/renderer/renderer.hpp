#pragma once

#include "../shared/common.hpp"

#include "../world/managers/map_manager.hpp"

#include "../world/components/animated_component.hpp"

#include "../world/systems/overlay_system.hpp"
#include "../world/systems/province_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/settlement_system.hpp"

#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "clay/clay_renderer_raylib.c"

#include <raylib.h>


namespace Renderer {

  inline Shader shader;
  inline Shader outline_shader;

  inline void DrawActors( bool debug );
  inline void draw_map( Map::Mode );


  inline void Init() {
    shader = LoadShader( "assets/shaders/pixel.vs", "assets/shaders/pixel.fs" );
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
        Province::System::Draw( Global::state.camera );
      }
      EndShaderMode();
    }
    EndBlendMode();

    switch ( map_mode ) {
      case Map::Mode::BuildPreview://@todo
      case Map::Mode::Default:
        // Draw TransparentOverlay

        Overlay::System::draw_default();
        BeginShaderMode( shader );
        {
          Overlay::System::draw_borders();
          Selection::System::Draw( Global::texture_cache, true );
        }
        EndShaderMode();

        Overlay::System::draw_settlement_name();
        break;
      case Map::Mode::Terrain:
        // Do nothing
        break;
      case Map::Mode::Political:
        // Draw OpaqueOverlay

        Overlay::System::draw_political();
        BeginShaderMode( shader );
        {
          Overlay::System::draw_borders();
          Selection::System::Draw( Global::texture_cache, true );
        }
        Overlay::System::draw_settlement_name();
        EndShaderMode();
        break;
      case Map::Mode::Resources:
        // Draw Resources
        BeginShaderMode( shader );
        { Resource::System::Draw( Global::state.camera ); }
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
        // DrawActors(
        //   Global::state.gameState == GameState::EDITOR
        // );

        DrawActors( true );
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
        Texture2D texture =
          Global::texture_cache[hstr{ anim.sprite_id.c_str() }]->texture;

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
        Texture2D texture =
          Global::texture_cache[hstr{ anim.sprite_id.c_str() }]->texture;

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

      if ( debug &&
           Vector2Distance( actor.position, actor.destination ) > 0.5f ) {
        DrawLineEx( actor.position, actor.destination, 2, MAGENTA );
      }
    } );
  }

  inline void Custom_Clay_Raylib_Render(
    Clay_RenderCommandArray renderCommands,
    Font *fonts
  ) {
    for ( int j = 0; j < renderCommands.length; j++ ) {
      Clay_RenderCommand *renderCommand =
        Clay_RenderCommandArray_Get( &renderCommands, j );
      Clay_BoundingBox boundingBox = renderCommand->boundingBox;
      switch ( renderCommand->commandType ) {
        case CLAY_RENDER_COMMAND_TYPE_TEXT: {
          // Raylib uses standard C strings so isn't compatible with cheap slices, we need to clone the string to append null terminator
          Clay_TextRenderData *textData = &renderCommand->renderData.text;
          char *cloned = (char *) malloc( textData->stringContents.length + 1 );
          memcpy(
            cloned,
            textData->stringContents.chars,
            textData->stringContents.length
          );
          cloned[textData->stringContents.length] = '\0';
          Font fontToUse = fonts[textData->fontId];
          DrawTextEx(
            fontToUse,
            cloned,
            Vector2{ boundingBox.x, boundingBox.y },
            (float) textData->fontSize,
            (float) textData->letterSpacing,
            CLAY_COLOR_TO_RAYLIB_COLOR( textData->textColor )
          );
          free( cloned );
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
          Texture2D imageTexture =
            *(Texture2D *) renderCommand->renderData.image.imageData;

          Clay_Color tintColor =
            renderCommand->renderData.image.backgroundColor;
          if ( tintColor.r == 0 && tintColor.g == 0 && tintColor.b == 0 &&
               tintColor.a == 0 ) {
            tintColor = Clay_Color{ 255, 255, 255, 255 };
          }
          DrawTextureEx(
            imageTexture,
            Vector2{ boundingBox.x, boundingBox.y },
            0,
            boundingBox.width / (float) imageTexture.width,
            CLAY_COLOR_TO_RAYLIB_COLOR( tintColor )
          );
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
          BeginScissorMode(
            (int) roundf( boundingBox.x ),
            (int) roundf( boundingBox.y ),
            (int) roundf( boundingBox.width ),
            (int) roundf( boundingBox.height )
          );
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
          EndScissorMode();
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
          Clay_RectangleRenderData *config =
            &renderCommand->renderData.rectangle;
          if ( config->cornerRadius.topLeft > 0 ) {
            float radius = ( config->cornerRadius.topLeft * 2 ) /
                           (float) ( ( boundingBox.width > boundingBox.height )
                                       ? boundingBox.height
                                       : boundingBox.width );
            DrawRectangleRounded(
              Rectangle{
                boundingBox.x,
                boundingBox.y,
                boundingBox.width,
                boundingBox.height
              },
              radius,
              8,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->backgroundColor )
            );
          } else {
            DrawRectangle(
              boundingBox.x,
              boundingBox.y,
              boundingBox.width,
              boundingBox.height,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->backgroundColor )
            );
          }
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_BORDER: {
          Clay_BorderRenderData *config = &renderCommand->renderData.border;
          // Left border
          if ( config->width.left > 0 ) {
            DrawRectangle(
              (int) roundf( boundingBox.x ),
              (int) roundf( boundingBox.y + config->cornerRadius.topLeft ),
              (int) config->width.left,
              (int) roundf(
                boundingBox.height - config->cornerRadius.topLeft -
                config->cornerRadius.bottomLeft
              ),
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          // Right border
          if ( config->width.right > 0 ) {
            DrawRectangle(
              (int) roundf(
                boundingBox.x + boundingBox.width - config->width.right
              ),
              (int) roundf( boundingBox.y + config->cornerRadius.topRight ),
              (int) config->width.right,
              (int) roundf(
                boundingBox.height - config->cornerRadius.topRight -
                config->cornerRadius.bottomRight
              ),
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          // Top border
          if ( config->width.top > 0 ) {
            DrawRectangle(
              (int) roundf( boundingBox.x + config->cornerRadius.topLeft ),
              (int) roundf( boundingBox.y ),
              (int) roundf(
                boundingBox.width - config->cornerRadius.topLeft -
                config->cornerRadius.topRight
              ),
              (int) config->width.top,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          // Bottom border
          if ( config->width.bottom > 0 ) {
            DrawRectangle(
              (int) roundf( boundingBox.x + config->cornerRadius.bottomLeft ),
              (int) roundf(
                boundingBox.y + boundingBox.height - config->width.bottom
              ),
              (int) roundf(
                boundingBox.width - config->cornerRadius.bottomLeft -
                config->cornerRadius.bottomRight
              ),
              (int) config->width.bottom,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          if ( config->cornerRadius.topLeft > 0 ) {
            DrawRing(
              Vector2{
                roundf( boundingBox.x + config->cornerRadius.topLeft ),
                roundf( boundingBox.y + config->cornerRadius.topLeft )
              },
              roundf( config->cornerRadius.topLeft - config->width.top ),
              config->cornerRadius.topLeft,
              180,
              270,
              10,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          if ( config->cornerRadius.topRight > 0 ) {
            DrawRing(
              Vector2{
                roundf(
                  boundingBox.x + boundingBox.width -
                  config->cornerRadius.topRight
                ),
                roundf( boundingBox.y + config->cornerRadius.topRight )
              },
              roundf( config->cornerRadius.topRight - config->width.top ),
              config->cornerRadius.topRight,
              270,
              360,
              10,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          if ( config->cornerRadius.bottomLeft > 0 ) {
            DrawRing(
              Vector2{
                roundf( boundingBox.x + config->cornerRadius.bottomLeft ),
                roundf(
                  boundingBox.y + boundingBox.height -
                  config->cornerRadius.bottomLeft
                )
              },
              roundf( config->cornerRadius.bottomLeft - config->width.top ),
              config->cornerRadius.bottomLeft,
              90,
              180,
              10,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          if ( config->cornerRadius.bottomRight > 0 ) {
            DrawRing(
              Vector2{
                roundf(
                  boundingBox.x + boundingBox.width -
                  config->cornerRadius.bottomRight
                ),
                roundf(
                  boundingBox.y + boundingBox.height -
                  config->cornerRadius.bottomRight
                )
              },
              roundf( config->cornerRadius.bottomRight - config->width.bottom ),
              config->cornerRadius.bottomRight,
              0.1,
              90,
              10,
              CLAY_COLOR_TO_RAYLIB_COLOR( config->color )
            );
          }
          break;
        }
        case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
          Clay_CustomRenderData *config = &renderCommand->renderData.custom;
          CustomLayoutElement *customElement =
            (CustomLayoutElement *) config->customData;
          if ( !customElement )
            continue;
          switch ( customElement->type ) {
            case CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL: {
              Clay_BoundingBox rootBox =
                renderCommands.internalArray[0].boundingBox;
              float scaleValue = CLAY__MIN(
                CLAY__MIN( 1, 768 / rootBox.height ) *
                  CLAY__MAX( 1, rootBox.width / 1024 ),
                1.5f
              );
              Ray positionRay = GetScreenToWorldPointWithZDistance(
                Vector2{
                  renderCommand->boundingBox.x +
                    renderCommand->boundingBox.width / 2,
                  renderCommand->boundingBox.y +
                    ( renderCommand->boundingBox.height / 2 ) + 20
                },
                Raylib_camera,
                (int) roundf( rootBox.width ),
                (int) roundf( rootBox.height ),
                140
              );
              BeginMode3D( Raylib_camera );
              DrawModel(
                customElement->customData.model.model,
                positionRay.position,
                customElement->customData.model.scale * scaleValue,
                WHITE
              );// Draw 3d model with texture
              EndMode3D();
              break;
            }
            default:
              break;
          }
          break;
        }
        default: {
          printf( "Error: unhandled render command." );
          exit( 1 );
        }
      }
    }
  }


};// namespace Renderer
