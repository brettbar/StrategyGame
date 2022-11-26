#pragma once

#include "../global.hpp"
#include "ui_lookups.hpp"
#include <iostream>
#include <memory>
#include <variant>

namespace UI {

typedef struct Element Element;
typedef struct Panel Panel;
// typedef struct TextLabel TextLabel;
// typedef struct TextButton TextButton;
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


// struct TextLabel : Element {
//   std::string text;
//   i32 font_size;
//   Color text_color;
//   bool dynamic = false;

//   TextLabel(
//     std::string id,
//     Color background,
//     std::string text,
//     i32 font_size,
//     Color text_color,
//     bool dynamic
//   )
//       : Element( id, Type::TextLabel, background, false, {}, {} ), text( text ),
//         font_size( font_size ), text_color( text_color ), dynamic( dynamic ) {
//   }

//   void Resize() {
//     const vec2 text_dims = MeasureTextEx(
//       Global::font_cache[hstr{ "font_romulus" }]->font,
//       text.c_str(),
//       font_size,
//       2.0f
//     );

//     transform.width = text_dims.x;
//     transform.height = text_dims.y;
//   }

//   void Update() {
//     if ( dynamic ) {
//       text = update_lookup.at( id )();
//     }
//   }
// };

// struct TextButton : TextLabel {
//   bool clickable = false;

//   void Update() {
//     TextLabel::Update();

//     this->clickable = clickable_lookup.at( id )();
//   }
// };

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
  bool absolute_pos = false;
  bool resizeable = false;
  std::function<Vector2()> update_pos;
  std::function<Vector2()> update_size;
  std::vector<entt::entity> children;

  static entt::entity Create(
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
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      absolute_pos,
      resizeable,
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
    if ( absolute_pos ) {
      Vector2 new_pos = update_pos();
      elem.transform.x = new_pos.x;
      elem.transform.y = new_pos.y;
    }
  }

  void Resize() {
    if ( resizeable ) {
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
        children_vert_align( children_vert_align ),
        absolute_pos( absolute_pos ), resizeable( resizeable ),
        update_pos( update_pos ), update_size( update_size ),
        children( children ) {
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
        children_vert_align( children_vert_align ), absolute_pos( false ),
        resizeable( false ), update_pos( {} ), update_size( {} ),
        children( children ){};
};

template<typename T>
static T &Get( entt::entity entity ) {
  assert( Global::local.all_of<T>( entity ) );
  auto &got = Global::local.get<T>( entity );
  return got;
}

inline Type GetType( entt::entity entity ) {
  if ( Global::local.all_of<Panel>( entity ) ) {
    return Global::local.get<Panel>( entity ).elem.type;
  }
  // if ( Global::local.all_of<TextLabel>( entity ) ) {
  //   return Type::TextLabel;
  // }

  if ( Global::local.all_of<TextureLabel>( entity ) ) {
    return Global::local.get<TextureLabel>( entity ).elem.type;
  }

  // if ( Global::local.all_of<TextButton>( entity ) ) {
  //   return Type::TextButton;
  // }

  if ( Global::local.all_of<TextureButton>( entity ) ) {
    return Global::local.get<TextureButton>( entity ).label.elem.type;
  }

  return Type::INVALID_TYPE;
}

inline rect &GetTransform( entt::entity entity ) {
  if ( Global::local.all_of<Panel>( entity ) ) {
    return Global::local.get<Panel>( entity ).elem.transform;
  }
  if ( Global::local.all_of<TextureLabel>( entity ) ) {
    return Global::local.get<TextureLabel>( entity ).elem.transform;
  }
  if ( Global::local.all_of<TextureButton>( entity ) ) {
    return Global::local.get<TextureButton>( entity ).label.elem.transform;
  }
  std::cout << "Invalid Transform!!" << std::endl;
}


inline void ToggleElem( entt::entity entity, bool on ) {
  if ( Global::local.all_of<Panel>( entity ) ) {
    printf( "Enabling panel\n" );
    Get<Panel>( entity ).elem.enabled = on;
    return;
  }

  if ( Global::local.all_of<TextureLabel>( entity ) ) {
    Get<TextureLabel>( entity ).elem.enabled = on;
    return;
  }

  if ( Global::local.all_of<TextureButton>( entity ) ) {
    Get<TextureButton>( entity ).label.elem.enabled = on;
    return;
  }

  // if ( Global::local.all_of<TextLabel>( entity ) ) {
  //   Get<TextLabel>( entity ).elem.enabled = on;
  //   return;
  // }

  // if ( Global::local.all_of<TextButton>( entity ) ) {
  //   Get<TextButton>( entity ).elem.enabled = on;
  //   return;
  // }
}

inline bool IsEnabled( entt::entity entity ) {
  if ( Global::local.all_of<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.enabled;
  }

  if ( Global::local.all_of<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.enabled;
  }

  if ( Global::local.all_of<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.enabled;
  }

  return false;
}

inline std::string GetId( entt::entity entity ) {
  if ( Global::local.all_of<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.id;
  }

  if ( Global::local.all_of<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.id;
  }

  if ( Global::local.all_of<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.id;
  }

  return "INVALID_ID";
}

inline bool IsInteractive( entt::entity entity ) {
  return Global::local.all_of<TextureButton>( entity );
}

inline bool IsClickable( entt::entity entity ) {
  if ( Global::local.all_of<TextureButton>( entity ) ) {
    // return Get<TextureButton>( entity ).clickable;
    return true;
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
