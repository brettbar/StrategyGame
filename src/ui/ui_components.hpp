#pragma once

#include "../global.hpp"
#include "ui_lookups.hpp"
#include <iostream>
#include <variant>

namespace UI {

typedef struct Panel Panel;
typedef struct TextLabel TextLabel;
typedef struct TextButton TextButton;
typedef struct TextureLabel TextureLabel;
typedef struct TextureButton TextureButton;

using Types =
  std::variant<Panel, TextLabel, TextButton, TextureLabel, TextureButton>;

inline f32 SCALE = 2.0f;
inline std::map<std::string, Types> lookup;

// struct Context {
//   entt::entity hot;
//   entt::entity active;
// };

struct Context {
  std::string hot;
  std::string active;
};

enum class Type {
  BasePanel,
  Panel,
  TextLabel,
  TextureLabel,
  TextButton,
  TextureButton,
};

enum class Axis {
  ROW,
  COLUMN,
};

enum class Align {
  START,
  CENTER,
  END,
  SPACE_OUT,
};

struct Margins {
  u32 left;
  u32 right;
  u32 top;
  u32 bottom;
};

struct Element {
  std::string id;
  Type type;
  Color background;
  bool enabled;
  rect transform;
  Margins margins;

  Element(
    std::string id,
    Type type,
    Color background,
    bool enabled,
    rect transform,
    Margins margins
  )
      : id( id ), type( type ), background( background ), enabled( enabled ),
        transform( transform ), margins( margins ) {
  }
};

struct TextLabel : Element {
  std::string text;
  i32 font_size;
  Color text_color;
  bool dynamic = false;

  TextLabel(
    std::string id,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic
  )
      : Element( id, Type::TextLabel, background, false, {}, {} ), text( text ),
        font_size( font_size ), text_color( text_color ), dynamic( dynamic ) {
  }

  void Resize() {
    const vec2 text_dims = MeasureTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      text.c_str(),
      font_size,
      2.0f
    );

    transform.width = text_dims.x;
    transform.height = text_dims.y;
  }

  void Update() {
    if ( dynamic ) {
      text = update_lookup.at( id )();
    }
  }
};

struct TextButton : TextLabel {
  bool clickable = false;

  void Update() {
    TextLabel::Update();

    this->clickable = clickable_lookup.at( id )();
  }
};

struct TextureLabel : Element {
  Texture2D texture;

  TextureLabel( std::string id )
      : Element( id, Type::TextureLabel, WHITE, false, {}, {} ),
        texture( Global::texture_cache[hstr{ id.c_str() }]->texture ) {
    this->transform.x = texture.width;
    this->transform.y = texture.height;
  }

  void Resize() {
    transform.width = texture.width * UI::SCALE;
    transform.height = texture.height * UI::SCALE;
  }
};

struct TextureButton : TextureLabel {
  bool clickable = false;

  TextureButton( std::string id ) : TextureLabel( id ) {
  }

  void Draw() {
    DrawTextureEx( texture, { transform.x, transform.y }, 0.0, SCALE, WHITE );
  }

  void Update() {
    this->clickable = clickable_lookup.at( id )();
  }
};

struct Panel : Element {
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  bool absolute_pos = false;
  bool resizeable = false;
  std::function<Vector2()> update_pos;
  std::function<Vector2()> update_size;

  // NOTE: Have to add new types of elements to this
  std::vector<Types> children;

  // TODO this probably shouldnt exist, since a panel with no children doesnt make sense
//  Panel(
//    std::string id,
//    Color background,
//    Axis children_axis,
//    Align children_horiz_align,
//    Align children_vert_align
//  )
//      : Element( id, Type::Panel, background, false, {}, {} ),
//        children_axis( children_axis ),
//        children_horiz_align( children_horiz_align ),
//        children_vert_align( children_vert_align ), children( {} ) {
//    this->transform = { 500, 500, 200, 200 };
//  }

  // Relative panel
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::vector<Types> children
  )
      : Element( id, Type::Panel, background, false, {}, {} ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), children( children ){};

  // Absolute panel
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool absolute_pos,
    bool resizeable,
    std::function<Vector2()> update_pos,
    std::function<Vector2()> update_size,
    std::vector<Types> children
  )
      : Element( id, Type::Panel, background, false, { 0, 0, 500, 200 }, {} ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ),
        absolute_pos( absolute_pos ), resizeable( resizeable ),
        update_pos( update_pos ), update_size( update_size ),
        children( children ) {

    lookup.insert_or_assign( this->id, *this );
  }

  // entt::entity entity = Global::local.create();
  // Global::local.emplace<Element>( entity, *this );
  // lookup.insert_or_assign( this->id, entity );

  void Place() {
    if ( this->absolute_pos ) {
      Vector2 new_pos = update_pos();
      this->transform.x = new_pos.x;
      this->transform.y = new_pos.y;
    }
  }

  void Resize() {
    if ( this->resizeable ) {
      Vector2 new_size = update_size();
      this->transform.width = new_size.x;
      this->transform.height = new_size.y;
    }
  }

  void Draw() {
    DrawRectangleV(
      { transform.x, transform.y },
      { transform.width, transform.height },
      background
    );
  }
};


inline std::string GetId( Types elem ) {
  if ( std::holds_alternative<Panel>( elem ) ) {
    return std::get<Panel>( elem ).id;
  }

  if ( std::holds_alternative<TextLabel>( elem ) ) {
    return std::get<TextLabel>( elem ).id;
  }

  if ( std::holds_alternative<TextureButton>( elem ) ) {
    return std::get<TextureButton>( elem ).id;
  }

  std::cout << "Couldn't find the ID!!!" << std::endl;

  return "INVALID_ID";
}

inline rect &GetTransform( Types *elem ) {
  if ( std::holds_alternative<Panel>( *elem ) ) {
    return std::get<Panel>( *elem ).transform;
  }

  if ( std::holds_alternative<TextLabel>( *elem ) ) {
    return std::get<TextLabel>( *elem ).transform;
  }

  if ( std::holds_alternative<TextureButton>( *elem ) ) {
    return std::get<TextureButton>( *elem ).transform;
  }

  std::cout << "No Transform found!!!" << std::endl;
}

inline bool IsEnabled( Types elem ) {
  if ( std::holds_alternative<Panel>( elem ) ) {
    return std::get<Panel>( elem ).enabled;
  }

  if ( std::holds_alternative<TextureButton>( elem ) ) {
    return std::get<TextureButton>( elem ).enabled;
  }

  if ( std::holds_alternative<TextLabel>( elem ) ) {
    return std::get<TextLabel>( elem ).enabled;
  }

  std::cout << "IsEnabled(): Missing a type catch" << std::endl;
  return false;
}

inline bool IsInteractive( Types elem ) {
  if ( std::holds_alternative<TextButton>( elem ) ) {
    return true;
  }

  if ( std::holds_alternative<TextureButton>( elem ) ) {
    return true;
  }

  return false;
}

inline bool IsClickable( Types elem ) {
  if ( std::holds_alternative<TextureButton>( elem ) ) {
    return std::get<TextureButton>( elem ).clickable;
  }

  if ( std::holds_alternative<TextButton>( elem ) ) {
    return std::get<TextButton>( elem ).clickable;
  }

  return false;
}

inline void UpdateElem( Types &elem ) {
  if ( std::holds_alternative<TextureButton>( elem ) ) {
    std::get<TextureButton>( elem ).Update();
    return;
  }

  if ( std::holds_alternative<TextLabel>( elem ) ) {
    std::get<TextLabel>( elem ).Update();
    return;
  }

  if ( std::holds_alternative<TextButton>( elem ) ) {
    std::get<TextButton>( elem ).Update();
    return;
  }
}

inline void ToggleElem( Types &elem, bool on ) {
  if ( std::holds_alternative<Panel>( elem ) ) {
    printf( "Enabling panel\n" );
    std::get<Panel>( elem ).enabled = on;
    return;
  }

  if ( std::holds_alternative<TextureButton>( elem ) ) {
    std::get<TextureButton>( elem ).enabled = on;
    return;
  }

  if ( std::holds_alternative<TextLabel>( elem ) ) {
    std::get<TextLabel>( elem ).enabled = on;
    return;
  }

  if ( std::holds_alternative<TextButton>( elem ) ) {
    std::get<TextButton>( elem ).enabled = on;
    return;
  }
}


// template<typename T>
// inline entt::entity CreateElement( T component, Element elem ) {
//   entt::entity entity = Global::local.create();

//   Global::local.emplace<Element>( entity, elem );
//   Global::local.emplace<T>( entity, component );

//   lookup.insert_or_assign( elem.id, entity );

//   return entity;
// }

};// namespace UI
