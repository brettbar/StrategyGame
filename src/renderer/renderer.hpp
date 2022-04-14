#include "../common.hpp"
#include "../state.hpp"
#include "../world/systems/animation_system.hpp"
#include "../world/systems/map/map_system.hpp"
#include "../world/systems/map/province_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/ui/ui_system.hpp"
#include "textures.hpp"
#include <raylib.h>

namespace Renderer {

inline Shader shader;

inline void Init( State &state, TextureCache &cache ) {

  Shader shader;


  shader = LoadShader( "assets/shaders/pixel.vs", "assets/shaders/pixel.fs" );
  // shader = LoadShader( 0, 0 );

  state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( state.mapWidth * 128.0f ) / 2, ( state.mapHeight * 128.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };

  Renderer::shader = shader;
}


inline void Draw( State &state, entt::registry &reg, TextureCache &cache ) {
  BeginDrawing();
  {
    ClearBackground( DARKGRAY );


    // float current_zoom = state.camera.zoom;
    // // printf( "current_zoom %f\n", current_zoom );

    // int zoomLoc = GetShaderLocation( shader, "uZoom" );
    // SetShaderValue( shader, zoomLoc, &current_zoom, SHADER_UNIFORM_FLOAT );

    // int resLoc = GetShaderLocation( shader, "iResolution" );
    // Vector3 res = { (f32) GetScreenWidth(), (f32) GetScreenHeight(), 0 };
    // SetShaderValue( shader, resLoc, &res, SHADER_UNIFORM_VEC3 );
    // printf( "res_loc %i\n", resLoc );


    BeginMode2D( state.camera );
    {
      // BeginShaderMode( shader );

      // float tex_width = 128.0f;
      // float tex_height = 128.0f;
      // int wt_loc = GetShaderLocation( shader, "wt" );
      // int ht_loc = GetShaderLocation( shader, "ht" );
      // int proj_mat_loc = GetShaderLocation( shader, "matProjection" );


      // rlSetMatrixModelview( matView );
      // rlSetMatrixProjection( matProj );
      // Matrix matView = MatrixIdentity();

      // Matrix proj_mat = GetCameraMatrix2D( state.camera );

      // rlSetMatrixProjection( proj_mat );
      // SetShaderValue( shader, wt_loc, &tex_width, SHADER_UNIFORM_FLOAT );
      // SetShaderValue( shader, ht_loc, &tex_height, SHADER_UNIFORM_FLOAT );
      MapSystem::Draw( state, reg, cache );

      // EndShaderMode();

      // tex_width = 1024.0f;
      // tex_height = 1792.0f;
      // SetShaderValue( shader, wt_loc, &tex_width, SHADER_UNIFORM_FLOAT );
      // SetShaderValue( shader, ht_loc, &tex_height, SHADER_UNIFORM_FLOAT );
      AnimationSystem::Draw( reg, state.gameState == GameState::EDITOR );


      SelectionSystem::Draw( reg, cache, state.gameState == GameState::EDITOR );
    }
    EndMode2D();

    UI::Draw();
  }
  EndDrawing();
}


};// namespace Renderer


//  glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);   // Hint for mipmaps generation algorythm: GL_FASTEST, GL_NICEST, GL_DONT_CARE
//  glGenerateMipmap(GL_TEXTURE_2D);    // Generate mipmaps automatically
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Activate Trilinear filtering for mipmaps
//
//  *mipmaps = 1 + (int)floor(log(MAX(width, height))/log(2));
//  TRACELOG(RL_LOG_INFO, "TEXTURE: [ID %i] Mipmaps generated automatically, total: %i", id, *mipmaps);

//  int hid = cache.handle(hstr{"hexagon"})->texture.id;
//  int vid = cache.handle(hstr{"romanVillagerTexture"})->texture.id;

//  int *mipmap = new int[128];
//  rlGenTextureMipmaps(id, 128, 128, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, mipmap);
//  rlTextureParameters(id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
//  rlTextureParameters(id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_MIP_LINEAR);

//  rlTextureParameters(hid, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
//  rlTextureParameters(hid, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_LINEAR);
//
//  rlTextureParameters(vid, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
//  rlTextureParameters(vid, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_LINEAR);

//  int zoomLoc = GetShaderLocation(shader, "uZoom");
//  SetShaderValue(shader, zoomLoc, &state.camera.zoom, SHADER_UNIFORM_FLOAT);

// Matrix matProj =
//   MatrixOrtho( 0.0, GetScreenWidth(), GetScreenHeight(), 0.0, 0.0, 1.0 );
// Matrix matView = MatrixIdentity();

// rlSetMatrixModelview( matView );
// rlSetMatrixProjection( matProj );

// Shader shader = LoadShader( 0, 0 );


//  int zoomLoc = GetShaderLocation(shader, "uZoom");
//  SetShaderValue(shader, zoomLoc, &state.camera.zoom, SHADER_UNIFORM_FLOAT);

//  int matViewLoc = GetShaderLocation(shader, "matView");
//  int matProjectionLoc = GetShaderLocation(shader, "matProjection");
//
//  SetShaderValueMatrix(shader, matProjectionLoc, matProj);
//  SetShaderValueMatrix(shader, matViewLoc, matView);
