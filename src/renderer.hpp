#include "common.hpp"
#include "resource.hpp"
#include "rlgl.h"
#include "state.hpp"
#include "systems/animation.hpp"
#include "systems/map/map.hpp"
#include "systems/map/provinces.hpp"
#include "ui/ui.hpp"

namespace Renderer
{
  Shader shader;

  void Init(State &);
  void Draw(State &, entt::registry &, TextureCache &);
};// namespace Renderer

inline void Renderer::Init(State &state)
{

  Matrix matProj = MatrixOrtho(0.0, state.screenWidth, state.screenHeight, 0.0, 0.0, 1.0);
  Matrix matView = MatrixIdentity();

  Shader shader = LoadShader("assets/shaders/pixel.vs", "assets/shaders/pixel.fs");
//  Shader shader = LoadShader(0, 0);

  //  rlSetMatrixModelview(matView);
  //  rlSetMatrixProjection(matProj);

  int zoomLoc = GetShaderLocation(shader, "uZoom");

  SetShaderValue(shader, zoomLoc, &state.camera.zoom, SHADER_UNIFORM_FLOAT);

//  int matViewLoc = GetShaderLocation(shader, "matView");
//  int matProjectionLoc = GetShaderLocation(shader, "matProjection");
//
//  SetShaderValueMatrix(shader, matProjectionLoc, matProj);
//  SetShaderValueMatrix(shader, matViewLoc, matView);

  Renderer::shader = shader;
}


inline void Renderer::Draw(State &state, entt::registry &reg, TextureCache &cache)
{
  BeginDrawing();
  {
    ClearBackground(DARKGRAY);


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

    BeginShaderMode(shader);
      BeginMode2D(state.camera);
      {
        Map::Draw(state, reg, cache);
        Animation::Draw(reg, state.debug);
      }
      EndMode2D();
    EndShaderMode();
    //
    //    {
    //      UI::Update(state, reg);
    //      UI::Draw(state, reg);
    //    }
  }
  EndDrawing();
}
