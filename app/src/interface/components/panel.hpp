#pragma once

#include "element.hpp"

#include "../ui_manager.hpp"


namespace UI
{

  struct Panel : Element
  {
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    bool abs_size = false;
    std::function<void( Panel & )> update;
    std::vector<ptr<Element>> children;

    std::vector<Messages::ID> subscribed_messages;

    // Should be recursive
    void Enable() override
    {
      Element::Enable();
      Resize();

      for ( ptr<Element> child: children )
      {
        if ( child )
        {
          child->Enable();
          child->Resize();
        }
      }
    }

    void Disable() override
    {
      for ( ptr<Element> child: children )
      {
        if ( child )
          child->Disable();
      }

      Element::Disable();
    }

    void Resize() override
    {
      if ( !IsEnabled() )
        return;

      f32 total_height = 0;
      f32 total_width = 0;
      f32 tallest_child = 0;
      f32 widest_child = 0;
      f32 end_of_last_x = transform.x;
      f32 end_of_last_y = transform.y;

      Update();

      for ( ptr<Element> child: children )
      {
        // TODO not sure if this is right
        // if ( !child )
        //   continue;
        if ( !child->IsEnabled() )
          continue;

        child->Resize();
      }

      if ( !abs_size )
      {
        for ( ptr<Element> child: children )
        {
          if ( !child->IsEnabled() )
            continue;

          total_width += child->transform.width;
          total_height += child->transform.height;

          if ( child->transform.width > widest_child )
            widest_child = child->transform.width;

          if ( child->transform.height > tallest_child )
            tallest_child = child->transform.height;
        }

        if ( children_axis == Axis::Row )
        {
          transform.width = total_width;
          transform.height = tallest_child;
        }
        else if ( children_axis == Axis::Column )
        {
          transform.width = widest_child;
          transform.height = total_height;
        }
      }


      for ( ptr<Element> child: children )
      {
        if ( !child->IsEnabled() )
          continue;

        if ( children_axis == Axis::Row )
        {
          // 2. Set the child x position based on alignment style.
          switch ( children_horiz_align )
          {
            case Align::Start:
            {
              child->transform.x = end_of_last_x + child->margins.left;
              end_of_last_x = child->transform.x + child->transform.width +
                              child->margins.right;
            }
            break;
          }

          // 3. Set the child y position based on alignment style.
          switch ( children_vert_align )
          {
            case Align::Start:
            {
              child->transform.y = transform.y;
            }
            break;
          }
        }
        else if ( children_axis == Axis::Column )
        {
          // 2. Set the child x position based on alignment style.
          switch ( children_horiz_align )
          {
            case Align::Start:
            {
              child->transform.x = transform.x;
            }
            break;
          }

          // 3. Set the child y position based on alignment style.
          switch ( children_vert_align )
          {
            case Align::Start:
            {
              child->transform.y = end_of_last_y;
              // + margins.top;
              end_of_last_y = child->transform.y + child->transform.height;
              // + margins.bottom;
            }
            break;
          }
        }
      }
    }

    void Reposition() override {}

    void Draw() override
    {
      if ( !IsEnabled() )
        return;

      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
      );

      for ( ptr<Element> child: children )
      {
        if ( child )
          child->Draw();
      }
    }

    void Interact( bool mouse_went_up, bool mouse_went_down ) override
    {
      for ( ptr<Element> child: children )
      {
        if ( child && child->IsEnabled() )
        {
          child->Interact( mouse_went_up, mouse_went_down );
        }
      }
    }

    void SubscribeToMessages() override
    {
      Messages::dispatcher.sink<Messages::DataUnion>()
        .connect<&Panel::ReceivedMessage>( this );
    }

    void ReceivedMessage( const Messages::DataUnion &event )
    {
      printf( "%s\n", this->id.c_str() );

      for ( Messages::ID msg_id: subscribed_messages )
      {
        if ( msg_id == event.message_id )
        {
          switch ( event.type )
          {
            case Messages::EnabledUpdate:
              if ( event.on )
                Enable();
              else
                Disable();
              break;
            case Messages::TextUpdate:
              break;
            case Messages::BackgroundUpdate:
              break;
            default:
              printf( "Error :: Panel deos not support message type\n" );
              break;
          }
          break;
        }
      }
    }

    void Update()
    {
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
          update( update ), children( children )
    {
    }

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
          subscribed_messages( subscribed_messages )
    {
    }

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
  struct StackPanel : Element
  {
    u32 curr_index = 0;
    // bool abs_pos = false;
    // bool abs_size = false;
    // std::function<Vector2()> update_pos;
    // std::function<Vector2()> update_size;

    // TODO determine if this will always only be a Panel
    // as child
    std::vector<std::shared_ptr<Element>> children;


    void Enable() override
    {
      Element::Enable();

      children[curr_index]->Enable();
    }

    void Disable() override
    {
      children[curr_index]->Disable();

      Element::Disable();
    }

    std::string ID()
    {
      return id;
    }

    void Resize() override
    {
      children[curr_index]->transform.x = transform.x;
      children[curr_index]->transform.y = transform.y;
      children[curr_index]->Resize();
    }

    void Draw() override
    {
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

    void Interact( bool mouse_went_up, bool mouse_went_down ) override
    {
      if ( children[curr_index] )
      {
        children[curr_index]->Interact( mouse_went_up, mouse_went_down );

        if ( !Manager()->over_any_elem )
        {
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
