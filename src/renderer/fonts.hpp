#pragma once

#include "../common.hpp"

struct FontResource {
  const Font font;
};

using FontCache = entt::resource_cache<FontResource>;
struct FontLoader : entt::resource_loader<FontLoader, FontResource> {
  std::shared_ptr<FontResource> load( Font font ) const {
    // ...
    return std::shared_ptr<FontResource>( new FontResource{ font } );
  }
};
