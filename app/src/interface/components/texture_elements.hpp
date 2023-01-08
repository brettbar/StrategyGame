#pragma once

#include "../clickable.hpp"
#include "../updates.hpp"
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
      texture, { elem.transform.x, elem.transform.y }, 0.0, SCALE, WHITE
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
  bool always_clickable = false;
  std::function<void()> action;

  static entt::entity Create( std::string id, std::function<void()> action ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id, action );

    Global::local.emplace<TextureButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  static entt::entity Create(
    std::string id,
    bool always_clickable,
    std::function<void()> action
  ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id, action );
    button.always_clickable = always_clickable;
    button.clickable = always_clickable;

    Global::local.emplace<TextureButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    label.Draw();
  }

  void Update() {
    if ( !always_clickable )
      clickable = clickable_lookup.at( label.elem.id )();
  }

  void Action() {
    action();
  }

  private:
  TextureButton( std::string id, std::function<void()> action )
      : label( TextureLabel( id, Type::TextureButton ) ), action( action ) {}
};


};// namespace UI
