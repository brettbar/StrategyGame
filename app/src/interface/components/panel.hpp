#pragma once

#include "element.hpp"

#include "../ui_manager.hpp"


namespace UI {

  struct Panel : Element {
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    bool abs_size = false;
    std::function<void( Panel & )> update;
    std::vector<ptr<Element>> children;

    std::vector<Messages::ID> subscribed_messages;

    // Should be recursive
    void Enable() override {
      Element::Enable();

      for ( ptr<Element> child: children ) {
        if ( child )
          child->Enable();
      }
    }

    void Disable() override {
      for ( ptr<Element> child: children ) {
        if ( child )
          child->Disable();
      }

      Element::Disable();
    }

    void Draw() override {
      if ( !IsEnabled() )
        return;

      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
      );

      for ( ptr<Element> child: children ) {
        if ( child )
          child->Draw();
      }
    }

    void Interact( bool mouse_went_up, bool mouse_went_down ) override {
      for ( ptr<Element> child: children ) {
        if ( child && child->IsEnabled() ) {
          child->Interact( mouse_went_up, mouse_went_down );
        }
      }
    }

    void SubscribeToMessages() override {
      Messages::dispatcher.sink<Messages::UpdateEnabled>()
        .connect<&Panel::ReceiveUpdateEnabled>( this );
    }

    void ReceiveUpdateEnabled( const Messages::UpdateEnabled &event ) {
      printf( "%s\n", this->id.c_str() );

      for ( Messages::ID msg_id: subscribed_messages ) {
        if ( msg_id == event.message_id ) {
          if ( event.on )
            Enable();
          else
            Disable();
          break;
        }
      }
    }

    void Update() {
      if ( update )
        update( *this );
    }


    // Absolute panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      bool abs_size,
      std::function<void( Panel & )> update,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, rect{ 0, 0, 500, 200 }, Margins{} ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), abs_size( abs_size ),
          update( update ), children( children ) {}

    // Absolute panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      bool abs_size,
      std::vector<Messages::ID> subscribed_messages,
      std::function<void( Panel & )> update,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, rect{ 0, 0, 500, 200 }, Margins{} ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), abs_size( abs_size ),
          update( update ), children( children ),
          subscribed_messages( subscribed_messages ) {}

    // Relative panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      std::vector<std::shared_ptr<Element>> children = {}
    )
        : Element( id, background, false, {}, {} ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), update( {} ),
          children( children ){};
    //
    // Relative panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      Margins margins,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, {}, margins ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), update( {} ),
          children( children ){};
  };

  // NOTE For right now, StackPanels can only have Panels as their direct children
  struct StackPanel : Element {
    u32 curr_index = 0;
    // bool abs_pos = false;
    // bool abs_size = false;
    // std::function<Vector2()> update_pos;
    // std::function<Vector2()> update_size;

    // TODO determine if this will always only be a Panel
    // as child
    std::vector<std::shared_ptr<Element>> children;


    void Enable() override {
      Element::Enable();

      children[curr_index]->Enable();
    }

    void Disable() override {
      children[curr_index]->Disable();

      Element::Disable();
    }

    std::string ID() {
      return id;
    }

    void Draw() override {
      if ( !IsEnabled() )
        return;

      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
      );

      if ( children[curr_index] )
        children[curr_index]->Draw();
    }

    void Interact( bool mouse_went_up, bool mouse_went_down ) override {
      if ( children[curr_index] ) {
        children[curr_index]->Interact( mouse_went_up, mouse_went_down );

        if ( !Manager()->over_any_elem ) {
          Manager()->SetContextNull();
        }
      }
    }

    // Relative panel
    StackPanel(
      std::string id,
      Color background,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, {}, {} ), children( children ){};
  };

};// namespace UI
