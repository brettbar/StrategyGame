#pragma once

#include "../common.hpp"
#include "../global.hpp"

#include "../events.hpp"
#include "../systems/actor_system.hpp"

#include "ui_shared.hpp"

namespace UI {

struct Context {
  entt::entity hot;
  entt::entity active;
};

inline f32 SCALE = 2.0f;

template<typename T>
inline bool Has( entt::entity entity ) {
  return Global::local.all_of<T>( entity );
}

template<typename T>
inline T &Get( entt::entity entity ) {
  assert( Has<T>( entity ) );
  auto &got = Global::local.get<T>( entity );
  return got;
}


inline Type GetType( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.type;
  }

  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.type;
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
  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.transform;
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
    Get<Panel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<StackPanel>( entity ) ) {
    Get<StackPanel>( entity ).elem.enabled = on;
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
  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.enabled;
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

  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.id;
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

// TODO maybe this could be refactored to some sort of event queue
inline std::map<std::string, std::function<void()>> action_lookup = {
  {
    "actor_spawn_settlement_button",
    []() {
      printf( "Spawn Settlement clicked!!\n" );
      SettlementSystem::SpawnSettlement();

      // TODO handle this in main
      // Events::event_emitter.publish( Events::UIEvent{
      //   "actor_spawn_settlement_button",
      // } );
    },
  },
  {
    "settlement_context_tab_overview",
    []() {
      printf( "Settlement tab button pressed\n" );
      entt::entity foo = lookup.at( "settlement_context_tab_overview" );
      auto &button = Get<TextureButton>( foo );
      entt::entity content = lookup.at( "settlement_context_tab_overview" );
      StackPanel &sp = Get<StackPanel>( content );
      SwitchChild( sp, 0 );
    },
  },
  {
    "settlement_context_tab_population",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_resources",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_culture",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_religion",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_construction",
    []() {
      printf( "Settlement tab button pressed\n" );
      entt::entity foo = lookup.at( "settlement_context_tab_overview" );
      auto &button = Get<TextureButton>( foo );
      entt::entity content = lookup.at( "settlement_context_tab_overview" );
      StackPanel &sp = Get<StackPanel>( content );
      SwitchChild( sp, 0 );
    },
  },
  {
    "settlement_context_tab_garrison",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  // Main Menu
  {
    "main_menu_resume_game",
    []() {
      printf( "Resume\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_resume_game",
      } );
    },
  },
  {
    "main_menu_start_game",
    []() {
      printf( "Start\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_start_game",
      } );
    },
  },
  {
    "main_menu_load_game",
    []() {
      printf( "Load\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_load_game",
      } );
    },
  },
  {
    "main_menu_settings",
    []() { printf( "Settings\n" ); },
  },
  {
    "main_menu_exit_game",
    []() {
      printf( "ExitGame\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_exit_game",
      } );
    },
  },
  // Modal Menu
  {
    "modal_menu_load_game",
    []() {
      printf( "Load\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "modal_menu_load_game",
      } );
    },
  },
  {
    "modal_menu_save_game",
    []() {
      printf( "Save\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "modal_menu_save_game",
      } );
    },
  },
  {
    "modal_menu_settings",
    []() { printf( "Settings\n" ); },
  },
  {
    "modal_menu_save_exit_main",
    []() { printf( "SaveExitMain\n" ); },
  },
  {
    "modal_menu_exit_main",
    []() {
      printf( "ExitMain\n" );
      Events::event_emitter.publish( Events::UIEvent{ "modal_menu_exit_main" }
      );
    },
  },
  {
    "modal_menu_exit_game",
    []() {
      printf( "ExitGame\n" );
      CloseWindow();
    },
  },
};

inline std::map<std::string, std::function<bool()>> clickable_lookup = {
  {
    "actor_spawn_settlement_button",
    []() -> bool { return ActorSystem::ColonistCanPlaceSettlement(); },
  },
};

inline std::map<std::string, std::function<std::string()>> update_lookup = {
  {
    "settlement_name",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return settlement.name;
      }

      return "Uninhabited";
    },
  },
  {
    "settlement_population",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return std::to_string( settlement.population.current );
      }

      return "0";
    },
  },
  {
    "settlement_development",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return Settlement::development.at( settlement.development );
      }

      return "Uninhabited";
    },
  },
};


enum class Axis {
  Row,
  Column,
};

enum class Align {
  Start,
  Center,
  End,
  SpaceOut,
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
        transform( transform ), margins( margins ) {}
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

  // Absolute-Positioned
  static entt::entity CreateAbsPos(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::function<Vector2()> update_pos,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      true,
      false,
      update_pos,
      {},
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  // // Absolute-Sized
  static entt::entity CreateAbsSize(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
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
      false,
      true,
      {},
      update_size,
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  // Absolute-Sized and Positioned
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

  // Default Panel
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
        update_size( update_size ), children( children ) {}

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

struct StackPanel {
  Element elem;
  u32 curr_index = 0;
  // bool abs_pos = false;
  // bool abs_size = false;
  // std::function<Vector2()> update_pos;
  // std::function<Vector2()> update_size;

  // TODO determine if this will always only be a Panel
  // as child
  std::vector<entt::entity> children;

  static entt::entity Create(
    std::string id,
    Color background,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    StackPanel panel = StackPanel( id, background, children );

    Global::local.emplace<StackPanel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      elem.background
    );
  }


  private:
  // Relative panel
  StackPanel(
    std::string id,
    Color background,
    std::vector<entt::entity> children
  )
      : elem( Element( id, Type::StackPanel, background, false, {}, {} ) ),
        children( children ){};
};


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

  static entt::entity Create(
    std::string id,
    std::string text,
    i32 font_size,
    Color background,
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

  static entt::entity Create(
    std::string id,
    std::string text,
    i32 font_size,
    Color background,
    Color text_color,
    bool dynamic,
    bool always_clickable
  ) {
    entt::entity entity = Global::local.create();
    TextButton button =
      TextButton( id, background, text, font_size, text_color, dynamic );

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
        ) ) {}
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

  static entt::entity Create( std::string id ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id );

    Global::local.emplace<TextureButton>( entity, button );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  static entt::entity Create( std::string id, bool always_clickable ) {
    entt::entity entity = Global::local.create();
    TextureButton button = TextureButton( id );
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

  private:
  TextureButton( std::string id )
      : label( TextureLabel( id, Type::TextureButton ) ) {}
};


};// namespace UI
