#pragma once

#include "../global.hpp"
#include "ui_lookups.hpp"
#include <iostream>
#include <memory>
#include <variant>

namespace UI {

typedef struct Element Element;
typedef struct Panel Panel;
typedef struct TextLabel TextLabel;
typedef struct TextButton TextButton;
typedef struct TextureLabel TextureLabel;
typedef struct TextureButton TextureButton;

inline f32 SCALE = 2.0f;
inline std::map<std::string, entt::entity> lookup;

struct Context {
  entt::entity hot;
  entt::entity active;
};

// struct Context {
//   std::string hot;
//   std::string active;
// };

enum class Type {
  INVALID_TYPE,
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

struct TextLabel {
  Element elem;
  std::string text;
  i32 font_size;
  Color text_color;
  bool dynamic = false;

  static entt::entity Create(
    std::string id,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic
  ) {
    entt::entity entity = Global::local.create();
    TextLabel button = TextLabel(
      id,
      Type::TextLabel,
      background,
      text,
      font_size,
      text_color,
      dynamic
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
        font_size( font_size ), text_color( text_color ), dynamic( dynamic ) {
  }
};

struct TextButton {
  TextLabel label;
  bool clickable = false;

  static entt::entity Create(
    std::string id,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic
  ) {
    entt::entity entity = Global::local.create();
    TextButton button =
      TextButton( id, background, text, font_size, text_color, dynamic );

    Global::local.emplace<TextButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    label.Draw();
    if ( !clickable ) {
      DrawRectangleV(
        { label.elem.transform.x, label.elem.transform.y },
        { label.elem.transform.width, label.elem.transform.height },
        Fade( BLACK, 0.7 )
      );
    }
  }


  void Update() {
    label.Update();

    clickable = clickable_lookup.at( label.elem.id )();
  }

  private:
  TextButton(
    std::string id,
    Color background,
    std::string text,
    i32 font_size,
    Color text_color,
    bool dynamic
  )
      : label( TextLabel(
          id,
          Type::TextButton,
          background,
          text,
          font_size,
          text_color,
          dynamic
        ) ) {
  }
};

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

struct Panel {
  Element elem;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  bool abs_pos = false;
  bool abs_size = false;
  std::function<Vector2()> update_pos;
  std::function<Vector2()> update_size;
  std::vector<entt::entity> children;

  static entt::entity Create(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool abs_pos,
    bool abs_size,
    std::function<Vector2()> update_pos,
    std::function<Vector2()> update_size,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      abs_pos,
      abs_size,
      update_pos,
      update_size,
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }


  static entt::entity Create(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }


  void Place() {
    if ( abs_pos ) {
      Vector2 new_pos = update_pos();
      elem.transform.x = new_pos.x;
      elem.transform.y = new_pos.y;
    }
  }

  void Resize() {
    if ( abs_size ) {
      Vector2 new_size = update_size();
      elem.transform.width = new_size.x;
      elem.transform.height = new_size.y;
    }
  }

  void Draw() {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      elem.background
    );
  }

  private:
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
    std::vector<entt::entity> children
  )
      : elem(
          Element( id, Type::Panel, background, false, { 0, 0, 500, 200 }, {} )
        ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), abs_pos( absolute_pos ),
        abs_size( resizeable ), update_pos( update_pos ),
        update_size( update_size ), children( children ) {
  }

  // Relative panel
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::vector<entt::entity> children
  )
      : elem(
          Element( id, Type::Panel, background, false, { 0, 0, 80, 200 }, {} )
        ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), abs_pos( false ),
        abs_size( false ), update_pos( {} ), update_size( {} ),
        children( children ){};
};

template<typename T>
static bool Has( entt::entity entity ) {
  return Global::local.all_of<T>( entity );
}

template<typename T>
static T &Get( entt::entity entity ) {
  assert( Has<T>( entity ) );
  auto &got = Global::local.get<T>( entity );
  return got;
}

inline Type GetType( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.type;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.type;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.type;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.type;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.type;
  }

  return Type::INVALID_TYPE;
}

inline rect &GetTransform( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.transform;
  }
  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.transform;
  }
  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.transform;
  }
  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.transform;
  }
  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.transform;
  }

  std::cout << "GetTransform() Invalid Transform!!" << std::endl;
  assert( false );
}

inline void UpdateElem( entt::entity entity ) {
  if ( Has<TextLabel>( entity ) ) {
    Get<TextLabel>( entity ).Update();
    return;
  }
  if ( Has<TextButton>( entity ) ) {
    Get<TextButton>( entity ).Update();
    return;
  }
}

inline void ToggleElem( entt::entity entity, bool on ) {
  if ( Has<Panel>( entity ) ) {
    printf( "Enabling panel\n" );
    Get<Panel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextLabel>( entity ) ) {
    Get<TextLabel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextButton>( entity ) ) {
    Get<TextButton>( entity ).label.elem.enabled = on;
    return;
  }

  if ( Has<TextureLabel>( entity ) ) {
    Get<TextureLabel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextureButton>( entity ) ) {
    Get<TextureButton>( entity ).label.elem.enabled = on;
    return;
  }
}

inline bool IsEnabled( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.enabled;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.enabled;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.enabled;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.enabled;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.enabled;
  }

  return false;
}

inline std::string GetId( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.id;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.id;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.id;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.id;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.id;
  }

  return "INVALID_ID";
}

inline bool IsInteractive( entt::entity entity ) {
  return Has<TextureButton>( entity ) || Has<TextButton>( entity );
}

inline bool IsClickable( entt::entity entity ) {
  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).clickable;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).clickable;
  }

  return false;
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
