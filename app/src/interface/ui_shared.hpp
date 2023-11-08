#pragma once
#include "../shared/global.hpp"
#include "element.hpp"

namespace UI
{

  struct Context
  {
    std::string hot;
    std::string active;
  };

  inline f32 SCALE = 1.0f;

  struct Element;
  inline std::map<std::string, sptr<Element>> lookup;


};// namespace UI
