#pragma once

#include "../include/entt/entt.hpp"
#include "common.hpp"
#include <raylib.h>

struct TextureResource {
  const Texture2D texture;
};

struct TextureLoader : entt::resource_loader<TextureLoader, TextureResource> {
  std::shared_ptr<TextureResource> load(Texture2D texture) const
  {
    // ...
    return std::shared_ptr<TextureResource>(new TextureResource{texture});
  }
};

using TextureCache = entt::resource_cache<TextureResource>;

inline void LoadResource(hstr id, str path, TextureCache &cache)
{
  Texture2D tex = LoadTexture(path.c_str());
  //  SetTextureFilter(tex, TEXTURE_WRAP_REPEAT);
  cache.load<TextureLoader>(id, tex);
}
