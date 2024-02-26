#pragma once

#include "common.hpp"
#include <raylib.h>

struct TextureResource {
  const Texture2D texture;
};

struct TextureLoader final : entt::resource_loader<TextureResource> {
  std::shared_ptr<TextureResource> operator()( Texture2D texture ) const {
    // ...
    return std::shared_ptr<TextureResource>( new TextureResource{ texture } );
  }
};

using TextureCache = entt::resource_cache<TextureResource, TextureLoader>;

inline void LoadAsset( hstr id, Image image, TextureCache &cache ) {
  ImageAlphaPremultiply( &image );
  Texture2D tex = LoadTextureFromImage( image );
  SetTextureFilter( tex, TEXTURE_FILTER_BILINEAR );
  cache.load( id, tex );
}


inline void LoadTexturePointFilter(
  hstr id,
  Image image,
  TextureCache &cache
) {
  ImageAlphaPremultiply( &image );
  Texture2D tex = LoadTextureFromImage( image );
  // SetTextureFilter( tex, TEXTURE_FILTER_BILINEAR );
  cache.load( id, tex );
}

inline Image CropUnitImage( std::string location ) {
  Image img = LoadImage( location.c_str() );
  ImageCrop( &img, { 32, 32, 64, 64 } );
  ImageAlphaPremultiply( &img );
  return img;
}

inline Image create_border(
  Vector2 start,
  Vector2 end,
  Color color,
  TextureCache &cache
) {
  Image base = GenImageColor( 65, 65, ColorAlpha( WHITE, 0.0 ) );
  ImageDrawLineV( &base, start, end, color );
  return base;
}

// inline void create_hex_texture(
//   hstr id,
//   Color color,
//   f32 padding,
//   TextureCache &cache
// ) {
//   RenderTexture2D target = LoadRenderTexture( TILE_WIDTH + 2, TILE_HEIGHT + 2 );
//   BeginTextureMode( target );
//   BeginDrawing();
//   ClearBackground( BLANK );

//   Vector4 edges[6] = {
//     { 32, 0, 64, 16 }, // ne
//     { 64, 16, 64, 48 },// e
//     { 64, 48, 32, 64 },// se
//     { 32, 64, 0, 48 }, // sw
//     { 0, 48, 0, 16 },  // w
//     { 0, 16, 32, 0 },  // nw
//   };

//   for ( auto edge: edges ) {
//     DrawTriangle(
//       { edge.x + 1, edge.y + 1 },
//       { 32 + 1, 32 + 1 },
//       { edge.z + 1, edge.w + 1 },
//       color
//     );
//   }


//   EndDrawing();
//   EndTextureMode();

//   cache.load( id, target.texture );

//   // Draw the texture with padding at the specified position
//   // DrawTexture(
//   //   target.texture, position.x - padding, position.y - padding, WHITE
//   // );
// }

inline void create_hex_texture( hstr id, Color color, TextureCache &cache ) {
  RenderTexture2D target = LoadRenderTexture( TILE_WIDTH, TILE_HEIGHT );
  BeginTextureMode( target );
  BeginDrawing();
  ClearBackground( BLANK );

  DrawTriangle( { 32, 0 }, { 32, 32 }, { 64, 16 }, color ); // ne
  DrawTriangle( { 64, 16 }, { 32, 32 }, { 64, 48 }, color );// e
  DrawTriangle( { 64, 48 }, { 32, 32 }, { 32, 64 }, color );// se
  DrawTriangle( { 32, 64 }, { 32, 32 }, { 0, 48 }, color ); // sw
  DrawTriangle( { 0, 48 }, { 32, 32 }, { 0, 16 }, color );  // w
  DrawTriangle( { 0, 16 }, { 32, 32 }, { 32, 0 }, color );  // nw

  EndDrawing();
  EndTextureMode();

  cache.load( id, target.texture );
}
