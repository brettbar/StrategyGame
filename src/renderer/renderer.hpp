#include "../common.hpp"
#include "../state.hpp"
#include "../world/systems/animation_system.hpp"
#include "../world/systems/map/map_system.hpp"
#include "../world/systems/selection_system.hpp"
#include "../world/systems/ui/ui_system.hpp"
#include "fonts.hpp"
#include "textures.hpp"
#include <raylib.h>

namespace Renderer {

inline Shader shader;

inline void Init( State &state, TextureCache &cache ) {

  Renderer::shader = LoadShader( "assets/shaders/pixel.vs", "assets/shaders/pixel.fs" );

  state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( state.mapWidth * 128.0f ) / 2, ( state.mapHeight * 128.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };
}


inline void Draw( State &state, entt::registry &reg, TextureCache &cache, FontCache &font_cache) {
  BeginDrawing();
  {
    ClearBackground( DARKGRAY );

    BeginMode2D( state.camera );
    {
      BeginShaderMode( shader );
      {
        MapSystem::Draw( state, reg, cache );


        AnimationSystem::Draw(
          reg,
          state.gameState == GameState::EDITOR );


      }
      EndShaderMode();

      SelectionSystem::Draw( reg, cache, state.gameState == GameState::EDITOR );
    }
    EndMode2D();

    UI::Draw(font_cache);
  }
  EndDrawing();
}




};// namespace Renderer
