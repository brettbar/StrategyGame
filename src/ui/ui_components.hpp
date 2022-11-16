#pragma once

#include "../global.hpp"

namespace UI {

inline std::map<std::string, entt::entity> lookup;

inline f32 SCALE = 2.0f;

struct Context {
  entt::entity hot;
  entt::entity active;
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
  bool enabled;
  rect transform;
  Margins margins;

  Element(
    std::string id,
    Type type,
    bool enabled,
    rect transform,
    Margins margins
  )
      : id( id ), type( type ), enabled( enabled ), transform( transform ),
        margins( margins ) {}
};

struct Panel : Element {
  Color background;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  bool absolute_pos = false;
  std::vector<Element> children;

  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    Element children...
  )
      : Element( id, Type::Panel, true, {}, {} ), background( background ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), children( { children } ) {}

  // TODO this probably shouldnt exist, since a panel with no children doesnt make sense
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align
  )
      : Element( id, Type::Panel, true, {}, {} ), background( background ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), children( {} ) {}

  // Absolute panel with children
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool absolute_pos,
    rect transform,
    Element children...
  )
      : Element( id, Type::Panel, true, transform, {} ),
        background( background ), children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ),
        absolute_pos( absolute_pos ), children( { children } ) {}

  //   // entt::entity entity = Global::local.create();
  //   // Global::local.emplace<Element>( entity, *this );
  //   // lookup.insert_or_assign( this->id, entity );
  // }
};

// struct BasePanel : Panel {
//   vec2 original_size;
//   std::function<void( Element &, BasePanel & )> update;
// };

// struct TextLabel : Element {
//   std::string text;
//   i32 font_size;
//   Color text_color;
//   Color background;
//   bool dynamic = false;

//   TextLabel(
//     std::string id,
//     std::string text,
//     i32 font_size,
//     Color text_color,
//     Color background
//   ) {
//     this->text = text;
//     this->font_size = font_size;
//     this->text_color = text_color;
//     this->background = background;
//   }
// };

// struct TextButton : TextLabel {
//   bool clickable;
// };

// struct TextureLabel : Element {
//   Texture2D texture;

//   TextureLabel( std::string id ) {
//     this->texture = Global::texture_cache[hstr{ id.c_str() }]->texture;
//   }
// };

// struct TextureButton : TextureLabel {
//   bool clickable;
// };

// template<typename T>
// inline entt::entity CreateElement( T component, Element elem ) {
//   entt::entity entity = Global::local.create();

//   Global::local.emplace<Element>( entity, elem );
//   Global::local.emplace<T>( entity, component );

//   lookup.insert_or_assign( elem.id, entity );

//   return entity;
// }

};// namespace UI
