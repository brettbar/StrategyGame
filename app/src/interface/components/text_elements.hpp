#pragma once

#include "element.hpp"

#include "../clickable.hpp"
#include "../updates.hpp"

namespace UI {

struct TextLabel {
  Element elem;
  std::string text;
  i32 font_size;
  Color text_color;
  bool dynamic = false;

  static entt::entity Create(
    std::string id,
    std::string text,
    i32 font_size,
    Color background,
    Color text_color,
    bool dynamic
  ) {
    entt::entity entity = Global::local.create();
    TextLabel button = TextLabel(
      id, Type::TextLabel, background, text, font_size, text_color, dynamic
    );

    Global::local.emplace<TextLabel>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Resize() {
    const vec2 text_dims = MeasureTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      text.c_str(),
      font_size,
      2.0f
    );

    elem.transform.width = text_dims.x;
    elem.transform.height = text_dims.y;
  }

  void Update() {
    if ( dynamic ) {
      text = update_lookup.at( elem.id )();
    }
  }

  void Draw() {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      elem.background
    );

    DrawTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      text.c_str(),
      {
        elem.transform.x,
        elem.transform.y,
      },
      font_size,
      2.0,
      text_color
    );
  }

  void Draw( Color override_background ) {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      override_background
    );

    DrawTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      text.c_str(),
      {
        elem.transform.x,
        elem.transform.y,
      },
      font_size,
      2.0,
      text_color
    );
  }

  TextLabel(
    std::string id,
    Type type,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic
  )
      : elem( Element( id, type, background, false, {}, {} ) ), text( text ),
        font_size( font_size ), text_color( text_color ), dynamic( dynamic ) {}
};

struct TextButton {
  TextLabel label;
  bool clickable = false;
  bool always_clickable = false;
  std::function<void()> action;

  static entt::entity Create(
    std::string id,
    std::string text,
    i32 font_size,
    Color background,
    Color text_color,
    bool dynamic,
    std::function<void()> action
  ) {
    entt::entity entity = Global::local.create();
    TextButton button = TextButton(
      id, background, text, font_size, text_color, dynamic, action
    );

    Global::local.emplace<TextButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  static entt::entity Create(
    std::string id,
    std::string text,
    i32 font_size,
    Color background,
    Color text_color,
    bool dynamic,
    bool always_clickable,
    std::function<void()> action
  ) {
    entt::entity entity = Global::local.create();
    TextButton button = TextButton(
      id, background, text, font_size, text_color, dynamic, action
    );

    button.always_clickable = always_clickable;
    button.clickable = always_clickable;

    Global::local.emplace<TextButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    if ( !clickable )
      label.Draw( Fade( BLACK, 0.5 ) );
    else
      label.Draw();
  }


  void Update() {
    label.Update();

    if ( !always_clickable )
      clickable = clickable_lookup.at( label.elem.id )();
  }

  void Action() {
    action();
  }

  private:
  TextButton(
    std::string id,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic,
    std::function<void()> action
  )
      : label( TextLabel(
          id,
          Type::TextButton,
          background,
          text,
          font_size,
          text_color,
          dynamic
        ) ),
        action( action ) {}
};

};// namespace UI
