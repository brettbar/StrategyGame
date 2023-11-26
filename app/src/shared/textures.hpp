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

inline void create_hex_texture( hstr id, Color color, TextureCache &cache ) {
  RenderTexture2D target = LoadRenderTexture( TILE_WIDTH, TILE_HEIGHT );
  BeginTextureMode( target );
  BeginDrawing();
  ClearBackground( BLANK );

  // DrawPoly( Vector2{ TILE_WIDTH / 2, TILE_HEIGHT / 2 }, 6, 32, 0, color );
  DrawTriangle( { 0, 16 }, { 32, 32 }, { 32, 0 }, color );
  DrawTriangle( { 64, 16 }, { 32, 32 }, { 64, 48 }, color );
  DrawTriangle( { 32, 0 }, { 32, 32 }, { 64, 16 }, color );
  DrawTriangle( { 64, 48 }, { 32, 32 }, { 32, 64 }, color );
  DrawTriangle( { 32, 64 }, { 32, 32 }, { 0, 48 }, color );
  DrawTriangle( { 0, 48 }, { 32, 32 }, { 0, 16 }, color );

  EndDrawing();
  EndTextureMode();
  cache.load( id, target.texture );
}