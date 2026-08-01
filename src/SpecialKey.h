#pragma once

#include <array>
#include <string>

enum class SpecialKey {
  BACKSPACE,
  ENTER,
  ESCAPE,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  ARROW_UP,
  UNHANDLED,
  COUNT,
};

constexpr std::array<std::string, static_cast<size_t>( SpecialKey::COUNT )>
  SpecialKeyNames{
    "BACKSPACE",  "ENTER",       "ESCAPE",   "ARROW_DOWN",
    "ARROW_LEFT", "ARROW_RIGHT", "ARROW_UP", "UNHANDLED",
  };
