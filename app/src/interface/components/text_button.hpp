#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

#include "text_label.hpp"

namespace UI
{
  struct TextButton : TextLabel
  {
    bool clickable = true;

    InterfaceEvent::Data on_click;

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
      InterfaceEvent::event_emitter.publish( on_click );
      // switch ( on_click->type ) {
      //   case InterfaceEvent::Type::Basic: {
      //     InterfaceEvent::event_emitter.publish( *on_click );
      //   } break;
      //   case InterfaceEvent::Type::ButtonClick: {
      //     std::shared_ptr<InterfaceEvent::ButtonClick> button_click =
      //       std::dynamic_pointer_cast<InterfaceEvent::ButtonClick>( on_click );

      //     if ( button_click ) {
      //       printf(
      //         "Sending button click! %s %s\n",
      //         button_click->origin_id.c_str(),
      //         button_click->msg.c_str()
      //       );
      //       InterfaceEvent::event_emitter.publish( *button_click );
      //     }
      //   } break;
      //   case InterfaceEvent::Type::JoinLobby: {
      //     std::shared_ptr<InterfaceEvent::JoinLobby> join_lobby =
      //       std::dynamic_pointer_cast<InterfaceEvent::JoinLobby>( on_click );

      //     if ( join_lobby ) {
      //       InterfaceEvent::event_emitter.publish( *join_lobby );
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
      InterfaceEvent::Data event
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
      bool clickable,
      InterfaceEvent::Data event,
      std::vector<InterfaceUpdate::ID> subscribed_messages
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_messages
          ),
          clickable( clickable ), on_click( event )
    {
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      InterfaceEvent::Data event,
      std::vector<InterfaceUpdate::ID> subscribed_messages
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
