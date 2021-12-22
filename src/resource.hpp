#pragma once

#include <raylib.h>

#include "../include/entt/entt.hpp"
#include "common.hpp"

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
  cache.load<TextureLoader>(id, LoadTexture(path.c_str()));
}
