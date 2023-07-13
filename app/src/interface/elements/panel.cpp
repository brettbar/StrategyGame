#pragma once

#include "../element.hpp"

namespace UI
{
  void Element::PanelEnable()
  {
    assert( type == Type::Panel );
    enabled = true;
    PanelResize();
    RepositionRecursive();

    for ( Element &child: children )
    {
      child.Enable();
      child.ResizeRecursive();
      child.RepositionRecursive();
    }
  }

  void Element::PanelRegister()
  {
    assert( type == Type::Panel );
    lookup.emplace( id, std::make_shared<Element>( *this ) );

    for ( Element &child: children )
    {
      child.Register();
    }
  }

  void Element::PanelDisable()
  {
    assert( type == Type::Panel );

    for ( Element &child: children )
    {
      child.Disable();
    }

    enabled = false;
  }

  void Element::PanelResize()
  {
    assert( type == Type::Panel );

    f32 total_height = 0;
    f32 total_width = 0;
    f32 tallest_child = 0;
    f32 widest_child = 0;

    for ( Element &child: children )
    {
      if ( !child.enabled )
        continue;

      // First, do all the Fixed and Minimal Resizing
      switch ( child.type )
      {
        case Type::Panel:
          child.PanelResize();
          break;
        case Type::DataPanel:
          child.DataPanelResize();
          break;
        case Type::StackPanel:
          child.StackPanelResize();
          break;
        case Type::TextButton:
        case Type::TextLabel:
        {
          const vec2 text_dims = MeasureTextEx(
            Global::font_cache[hstr{ "font_romulus" }]->font,
            child.text.c_str(),
            child.font_size,
            2.0f
          );

          child.transform.width = text_dims.x;
          child.transform.height = text_dims.y;
        }
        break;
        case Type::TextureButton:
        case Type::TextureLabel:
        {
          child.transform.width = child.texture.width;
          child.transform.height = child.texture.height;
        }
        break;
      }

      total_width += child.transform.width;
      total_height += child.transform.height;

      if ( child.transform.width > widest_child )
        widest_child = child.transform.width;

      if ( child.transform.height > tallest_child )
        tallest_child = child.transform.height;
    }

    f32 total_fixed_or_min_width = 0;
    f32 total_fixed_or_min_height = 0;

    // Once we know the fixed and minimals, do the maximums
    for ( Element &child: children )
    {
      if ( !child.enabled )
        continue;

      if ( child.size == Size::Minimum || child.size == Size::Fixed )
      {
        total_fixed_or_min_width +=
          child.transform.width + child.margins.left + child.margins.right;
        total_fixed_or_min_height +=
          child.transform.height + child.margins.bottom + child.margins.top;
      }
    }

    for ( Element &child: children )
    {
      if ( !child.enabled )
        continue;

      if ( child.size == Size::Maximum )
      {
        child.transform.width = total_fixed_or_min_width / children.size();
        child.transform.height = total_fixed_or_min_height / children.size();
      }
    }

    if ( size == Size::Minimum )
    {
      if ( children_axis == Axis::Row )
      {
        transform.width = total_width;
        transform.height = tallest_child;
      }
      else
      {
        transform.width = widest_child;
        transform.height = total_height;
      }
    };
    // case Size::Maximum:
    // {
    //   f32 width_left = parent_width - final_sibling_width_total;
    //   f32 height_left = parent_height - final_sibling_height_total;

    //   transform.width = width_left / num_final_siblings;
    //   transform.height = width_height / num_final_siblings;
    // }
    // break;
  }

  void Element::PanelReposition()
  {
    assert( type == Type::Panel );
    f32 total_height = 0;
    f32 total_width = 0;
    f32 tallest_child = 0;
    f32 widest_child = 0;
    f32 end_of_last_x = transform.x;
    f32 end_of_last_y = transform.y;

    if ( children_axis == Axis::Row )
    {
      switch ( children_horiz_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.RepositionRecursive();

            child.transform.x = end_of_last_x + child.margins.left;
            end_of_last_x =
              child.transform.x + child.transform.width + child.margins.right;
          }
        }
        break;
        case Align::SpaceBetween:
        {
          for ( Element &child: children )
          {
            child.RepositionRecursive();

            f32 gap_width =
              ( this->transform.width - total_width ) / ( children.size() - 1 );

            child.transform.x = end_of_last_x + child.margins.left;
            end_of_last_x = child.transform.x + child.transform.width +
                            child.margins.right + gap_width;
          }
        }
        break;
      }

      switch ( children_vert_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.RepositionRecursive();
            child.transform.y = transform.y;
          }
        }
        break;
      }
    }
    else
    {// Axis::Column
      // 2. Set the child x position based on alignment style.
      switch ( children_horiz_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.RepositionRecursive();
            child.transform.x = transform.x;
          }
        }
        break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( children_vert_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.RepositionRecursive();
            child.transform.y = end_of_last_y;
            // + margins.top;
            end_of_last_y = child.transform.y + child.transform.height;
            // + margins.bottom;
          }
        }
        break;
      }
    }
  }


  void Element::PanelDraw()
  {
    assert( type == Type::Panel );

    DrawRectangleV(
      { transform.x, transform.y },
      { transform.width, transform.height },
      background
    );

    for ( Element &child: children )
    {
      child.Draw();
    }
  }


  void Element::PanelExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    assert( type == Type::Panel );
    if ( updates.contains( update.id ) )
      updates[update.id]( *this, update );

    for ( auto &child: children )
    {
      child.ExecuteInterfaceUpdate( update );
    }
  }
};// namespace UI