#include "../common.hpp"
#include "../state.hpp"
#include "../systems/animation_system.hpp"
#include "../systems/map/map_system.hpp"
#include "../systems/selection_system.hpp"
#include "../systems/ui/ui_system.hpp"
#include "../systems/ui/overlay_system.hpp"
#include "fonts.hpp"
#include "textures.hpp"
#include <raylib.h>

namespace Renderer {

inline Shader shader;

inline void Init( State &state ) {

  Renderer::shader = LoadShader( "assets/shaders/pixel.vs", "assets/shaders/pixel.fs" );

  state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( state.mapWidth * 128.0f ) / 2, ( state.mapHeight * 128.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };
}


inline void Draw( State &state, entt::registry &reg, TextureCache &texture_cache, FontCache &font_cache) {
  BeginDrawing();
  {
    ClearBackground( DARKGRAY );

    BeginMode2D( state.camera );

    BeginShaderMode( shader );
    Terrain::Draw( state.camera, reg, texture_cache );
    EndShaderMode();

    OverlaySystem::Draw(reg, texture_cache, font_cache);

    BeginShaderMode( shader );
    switch ( MapSystem::mode ) {
      case MapSystem::Mode::TERRAIN:
        ProvinceSystem::DrawProvinces( reg, texture_cache, false );
        break;
      case MapSystem::Mode::POLITICAL:
        ProvinceSystem::DrawProvinces( reg, texture_cache, true );
        break;
    }

    AnimationSystem::Draw(
      reg,
      state.gameState == GameState::EDITOR );

    EndShaderMode();




    SelectionSystem::Draw( reg, texture_cache, state.gameState == GameState::EDITOR );

    EndMode2D();

    UI::Draw(font_cache);
  }
  EndDrawing();
}




};// namespace Renderer
