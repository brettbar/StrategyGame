#pragma once

#include "element.hpp"

namespace UI {

struct TextureLabel {
  Element elem;
  Texture2D texture;

  static entt::entity Create( std::string id ) {
    entt::entity entity = Global::local.create();
    TextureLabel label = TextureLabel( id, Type::TextureLabel );

    Global::local.emplace<TextureLabel>( entity, label );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Resize() {
    elem.transform.width = texture.width * UI::SCALE;
    elem.transform.height = texture.height * UI::SCALE;
  }

  void Draw() {
    DrawTextureEx(
      texture,
      { elem.transform.x, elem.transform.y },
      0.0,
      SCALE,
      WHITE
    );
  }

  TextureLabel( std::string id, Type type )
      : elem( Element( id, type, WHITE, false, {}, {} ) ),
        texture( Global::texture_cache[hstr{ id.c_str() }]->texture ) {
    elem.transform.x = texture.width * UI::SCALE;
    elem.transform.y = texture.height * UI::SCALE;
  }
};

struct TextureButton {
  TextureLabel label;
  bool clickable = false;

  static entt::entity Create( std::string id ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id );

    Global::local.emplace<TextureButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  static entt::entity Create( std::string id, bool clickable ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id );
    button.clickable = clickable;

    Global::local.emplace<TextureButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    label.Draw();
  }

  void Update() {
    clickable = clickable_lookup.at( label.elem.id )();
  }

  private:
  TextureButton( std::string id )
      : label( TextureLabel( id, Type::TextureButton ) ) {
  }
};
};// namespace UI
