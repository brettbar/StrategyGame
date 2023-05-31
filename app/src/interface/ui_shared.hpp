#pragma once
#include "../shared/global.hpp"

namespace UI
{

  struct Context
  {
    std::string hot;
    std::string active;
  };

  inline f32 SCALE = 1.0f;

  class Element;
  inline std::map<std::string, ptr<Element>> lookup;

};// namespace UI
