#pragma once

#include "../common.hpp"
#include "rlgl.h"

struct TextureResource {
  const Texture2D texture;
};

struct TextureLoader : entt::resource_loader<TextureLoader, TextureResource> {
  std::shared_ptr<TextureResource> load( Texture2D texture ) const {
    // ...
    return std::shared_ptr<TextureResource>( new TextureResource{ texture } );
  }
};

using TextureCache = entt::resource_cache<TextureResource>;

inline void LoadResource( hstr id, Image image, TextureCache &cache ) {
  Texture2D tex = LoadTextureFromImage( image );

  // Set bilinear filtering on
  // rlSetTexture( tex.id );
  // rlTextureParameters(
  //   tex.id,
  //   RL_TEXTURE_MIN_FILTER,
  //   RL_TEXTURE_FILTER_LINEAR );

  // rlTextureParameters(
  //   tex.id,
  //   RL_TEXTURE_MAG_FILTER,
  //   RL_TEXTURE_FILTER_LINEAR );

  // Trilinear with mipmapping
  // int *mipmap = new int[128];
  // rlGenTextureMipmaps(
  //   tex.id,
  //   128,
  //   128,
  //   RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  //   mipmap );

  // rlTextureParameters(
  //   tex.id,
  //   RL_TEXTURE_MAG_FILTER,
  //   RL_TEXTURE_FILTER_LINEAR );

  // // Activate Trilinear filtering for mipmaps
  // rlTextureParameters(
  //   tex.id,
  //   RL_TEXTURE_MIN_FILTER,
  //   RL_TEXTURE_FILTER_MIP_LINEAR );

  // printf(
  //   "TEXTURE: [ID %i] Mipmaps generated automatically, total: %i\n",
  //   tex.id,
  //   *mipmap );

  cache.load<TextureLoader>( id, tex );
}
