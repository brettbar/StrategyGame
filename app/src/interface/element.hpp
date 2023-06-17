#pragma once

#include "../shared/global.hpp"
#include "../shared/utils.hpp"
#include "../signals/events.hpp"
#include "../signals/updates.hpp"
#include "ui_shared.hpp"

namespace UI
{
  enum class Type : u32
  {
    INVALID,
    Panel,
    DataPanel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
  };

  enum class Anchor
  {
    Free,
    TopLeft,
    TopMid,
    TopRight,
    MidLeft,
    Centered,
    MidRight,
    BottomLeft,
    BottomMid,
    BottomRight,
  };

  enum class Axis
  {
    INVALID,
    Row,
    Column,
  };

  enum class Align
  {
    INVALID,
    Start,
  };

  struct Margins
  {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  class Element
  {

    // Element() = delete;
    Element() = default;
    Element( std::string id ) : id( id ){};
    // ~Element() {
    //   // TODO Remove id from lookup
    // }

public:
    friend class PanelBuilder;
    friend class DataPanelBuilder;
    friend class StackPanelBuilder;
    friend class TextLabelBuilder;
    friend class TextButtonBuilder;
    friend class TextureLabelBuilder;
    friend class TextureButtonBuilder;

    Type type = Type::INVALID;
    std::string id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
    bool fixed_size = false;
    Color background = BLACK;
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    // Panel
    Anchor anchor = Anchor::Free;// Might apply to others as well?
    Axis children_axis = Axis::INVALID;
    Align children_horiz_align = Align::INVALID;
    Align children_vert_align = Align::INVALID;

    std::vector<Element> children = {};

    //DataPanel
    std::map<std::string, Element> data_points = {};
    std::function<void( Element & )> update = {};


    std::function<void( std::map<std::string, bool> &, std::vector<Element> & )>
      update_children = {};


    std::vector<InterfaceUpdate::ID> subscribed_updates = {};

    std::map<
      InterfaceUpdate::ID,
      std::function<void( Element &, InterfaceUpdate::Update )>>
      updates = {};

    // StackPanel
    // TODO this should probably be made private
    u32 curr_index = 0;

    // TextLabel
    std::string text = "INVALID";
    i32 font_size = 14;
    Color text_color = WHITE;
    // TODO(rf) probably can remove all together
    bool dynamic = false;

    // TextButton
    bool clickable = true;
    ptr<InterfaceEvent::Data> on_click = nullptr;

    // TextureLabel
    Texture2D texture = Texture2D();

    void InitialEnable();
    void Enable();
    void Register();
    void Disable();
    void Destroy();
    void SwitchChild( u32 );
    void Reposition();
    // TODO why are we Repositioning panel children in this function?
    // Shouldnt it be in Reposition fn?
    // TODO should we take in to account ui scale
    void Resize();
    void UpdateText( std::string );
    void UpdateBackground( Color );
    void UpdateClickable( bool new_clickable );
    void ExecuteInterfaceUpdate( const InterfaceUpdate::Update & );
    void UpdateChildren();
    void Draw();
    void FireEvent();

    void CreateElementForDatapoints( Element );
  };


};// namespace UI
