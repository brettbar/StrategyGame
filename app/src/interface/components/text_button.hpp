#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

#include "text_label.hpp"

namespace UI
{
  struct TextButton : TextLabel
  {
    bool always_clickable = true;
    bool clickable = true;

    Events::EventUnion on_click;

    void Draw() override
    {
      if ( !IsEnabled() )
        return;

      if ( !clickable )
        TextLabel::Draw( Fade( BLACK, 0.5 ) );
      else
        TextLabel::Draw();
    }

    void Interact( bool mouse_went_up, bool mouse_went_down ) override
    {
      bool inside = CheckCollisionPointRec( GetMousePosition(), transform );

      if ( !Manager()->over_any_elem )
        Manager()->over_any_elem = inside;

      if ( Manager()->DoInteraction(
             *this, inside, mouse_went_up, mouse_went_down
           ) )
      {
        std::cout << "INTERACTION DETECTED!!!" << std::endl;

        FireEvent();
      }
    }

    void FireEvent()
    {
      Events::event_emitter.publish( on_click );
      // switch ( on_click->type ) {
      //   case Events::Type::Basic: {
      //     Events::event_emitter.publish( *on_click );
      //   } break;
      //   case Events::Type::ButtonClick: {
      //     std::shared_ptr<Events::ButtonClick> button_click =
      //       std::dynamic_pointer_cast<Events::ButtonClick>( on_click );

      //     if ( button_click ) {
      //       printf(
      //         "Sending button click! %s %s\n",
      //         button_click->origin_id.c_str(),
      //         button_click->msg.c_str()
      //       );
      //       Events::event_emitter.publish( *button_click );
      //     }
      //   } break;
      //   case Events::Type::JoinLobby: {
      //     std::shared_ptr<Events::JoinLobby> join_lobby =
      //       std::dynamic_pointer_cast<Events::JoinLobby>( on_click );

      //     if ( join_lobby ) {
      //       Events::event_emitter.publish( *join_lobby );
      //     }
      //   } break;
      // }
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      Events::EventUnion event
    )
        : TextLabel( id, text, font_size, background, text_color ),
          on_click( event )
    {
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      Events::EventUnion event,
      std::vector<Messages::ID> subscribed_messages
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_messages
          ),
          on_click( event )
    {
    }
  };
};// namespace UI
