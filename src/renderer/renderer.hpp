#include "../common.hpp"
#include "../resource.hpp"
#include "../state.hpp"
#include "../world/systems/animation_system.hpp"
#include "../world/systems/map/map_system.hpp"
#include "../world/systems/map/province_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/ui/ui_system.hpp"

#include "rlgl.h"


namespace Renderer

{
Shader shader;


void Init( State & );

void Draw( State &, entt::registry &, TextureCache & );
};// namespace Renderer

inline void Renderer::Init( State &state )

{

  //  Matrix matProj = MatrixOrtho(0.0, state.screenWidth, state.screenHeight, 0.0, 0.0, 1.0);
  //  Matrix matView = MatrixIdentity();

  //  Shader shader = LoadShader("assets/shaders/pixel.vs", "assets/shaders/pixel.fs");
  Shader shader = LoadShader( 0, 0 );

  //  rlSetMatrixModelview(matView);
  //  rlSetMatrixProjection(matProj);


  //  int zoomLoc = GetShaderLocation(shader, "uZoom");
  //  SetShaderValue(shader, zoomLoc, &state.camera.zoom, SHADER_UNIFORM_FLOAT);

  //  int matViewLoc = GetShaderLocation(shader, "matView");
  //  int matProjectionLoc = GetShaderLocation(shader, "matProjection");
  //
  //  SetShaderValueMatrix(shader, matProjectionLoc, matProj);
  //  SetShaderValueMatrix(shader, matViewLoc, matView);
  Renderer::shader = shader;
}


inline void
Renderer::Draw( State &state, entt::registry &reg, TextureCache &cache )

{
  BeginDrawing();
  {
    ClearBackground( DARKGRAY );

    //    BeginShaderMode(shader);
    //      BeginMode2D(state.camera);
    //        Texture2D hex = cache.handle(hstr{"hexagon"})->texture;
    //        Rectangle frameRec = {1.0f, 1.0f, 128, 128};
    //        for (int i = 0; i < 10; i++)
    //        {
    //          for (int j = 0; j < 10; j++)
    //          {
    //            DrawTextureRec(hex, frameRec, {i * 128.0f, j * 128.0f}, WHITE);
    //          }
    //        }
    //      EndMode2D();
    //    EndShaderMode();

    //      glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);   // Hint for mipmaps generation algorythm: GL_FASTEST, GL_NICEST, GL_DONT_CARE
    //      glGenerateMipmap(GL_TEXTURE_2D);    // Generate mipmaps automatically
    //
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Activate Trilinear filtering for mipmaps
    //
    //      *mipmaps = 1 + (int)floor(log(MAX(width, height))/log(2));
    //      TRACELOG(RL_LOG_INFO, "TEXTURE: [ID %i] Mipmaps generated automatically, total: %i", id, *mipmaps);

    //    int hid = cache.handle(hstr{"hexagon"})->texture.id;
    //    int vid = cache.handle(hstr{"romanVillagerTexture"})->texture.id;

    //    int *mipmap = new int[128];
    //    rlGenTextureMipmaps(id, 128, 128, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, mipmap);
    //    rlTextureParameters(id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
    //    rlTextureParameters(id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_MIP_LINEAR);

    //    rlTextureParameters(hid, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
    //    rlTextureParameters(hid, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_LINEAR);
    //
    //    rlTextureParameters(vid, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_LINEAR);
    //    rlTextureParameters(vid, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_LINEAR);

    //    int zoomLoc = GetShaderLocation(shader, "uZoom");
    //    SetShaderValue(shader, zoomLoc, &state.camera.zoom, SHADER_UNIFORM_FLOAT);


    BeginShaderMode( shader );
    BeginMode2D( state.camera );
    {
      MapSystem::Draw( state, reg, cache );
      AnimationSystem::Draw( reg, state.gameState == GameState::EDITOR );
      SelectionSystem::Draw( reg, state.gameState == GameState::EDITOR );
    }
    EndMode2D();
    EndShaderMode();

    UI::Draw();
  }
  EndDrawing();
}


// if (state.gameState == EDITOR)

//   Editor::Draw();
