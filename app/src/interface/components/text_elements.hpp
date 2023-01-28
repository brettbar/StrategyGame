#pragma once

#include "element.hpp"

#include "../clickable.hpp"
#include "../updates.hpp"
//
// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

namespace UI {

  struct TextLabel {
    Element elem;
    std::string text;
    i32 font_size;
    Color text_color;
    bool dynamic = false;
    std::function<std::string()> update;

    std::string ID() {
      return elem.id;
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
      if ( elem.enabled && dynamic ) {
        if ( update_lookup.contains( elem.id ) ) {
          text = update_lookup.at( elem.id )();
        }
        else if ( update ) {
          text = update();
        }
        else {
          printf( "ERROR :: Update not found for elem %s\n", elem.id.c_str() );
        }
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
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic
    )
        : elem( Element( id, background, false, {}, {} ) ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( dynamic ) {
    }

    TextLabel(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      std::function<std::string()> update
    )
        : elem( Element( id, background, false, {}, {} ) ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( dynamic ),
          update( update ) {}
  };

  struct TextButton {
    TextLabel label;
    bool always_clickable = true;
    bool clickable = true;

    std::function<void()> action;


    std::string ID() {
      return label.ID();
    }

    void Draw() {
      if ( !clickable )
        label.Draw( Fade( BLACK, 0.5 ) );
      else
        label.Draw();
    }


    void Update() {
      label.Update();

      if ( label.elem.enabled && !always_clickable )
        clickable = clickable_lookup.at( label.elem.id )();
    }

    void Action() {
      action();
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic
    )
        : label(
            TextLabel( id, text, font_size, background, text_color, dynamic )
          ) {
      action = [id]() {
        Events::event_emitter.publish( Events::ButtonClick{ id } );
      };
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      std::function<void()> action
    )
        : label(
            TextLabel( id, text, font_size, background, text_color, dynamic )
          ),
          action( action ) {}

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      bool always_clickable
    )
        : label(
            TextLabel( id, text, font_size, background, text_color, dynamic )
          ),
          always_clickable( always_clickable ), clickable( always_clickable ) {

      action = [id]() {
        Events::event_emitter.publish( Events::ButtonClick{ id } );
      };
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      bool always_clickable,
      std::function<void()> action
    )
        : label(
            TextLabel( id, text, font_size, background, text_color, dynamic )
          ),
          action( action ) {}
  };
};// namespace UI
