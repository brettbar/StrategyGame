#pragma once

#include "../common.hpp"

struct FontResource {
  const Font font;
};

struct FontLoader final : entt::resource_loader<FontResource> {
  std::shared_ptr<FontResource> operator()( Font font ) {
    // ...
    return std::shared_ptr<FontResource>( new FontResource{ font } );

    // return std::make_shared<FontResource>( new FontResource{ font } );
  }
};

using FontCache = entt::resource_cache<FontResource, FontLoader>;
