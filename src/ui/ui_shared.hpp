#pragma once

namespace UI {
enum class Type {
  INVALID_TYPE,
  Panel,
  StackPanel,
  TextLabel,
  TextureLabel,
  TextButton,
  TextureButton,
};


typedef struct Element Element;
typedef struct Panel Panel;
typedef struct StackPanel StackPanel;
typedef struct TextLabel TextLabel;
typedef struct TextButton TextButton;
typedef struct TextureLabel TextureLabel;
typedef struct TextureButton TextureButton;

};// namespace UI
