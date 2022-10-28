#pragma once

#include "../common.hpp"

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

inline void LoadResource( hstr id, Image image, TextureCache &cache ) {
  ImageAlphaPremultiply( &image );
  Texture2D tex = LoadTextureFromImage( image );
  cache.load( id, tex );
}

inline void LoadResourceByFilename( std::string id, TextureCache &cache ) {
  std::string path = "assets/textures/" + id;
  LoadResource( hstr{ id.c_str() }, LoadImage( path.c_str() ), cache );
}
