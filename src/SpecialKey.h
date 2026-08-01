#pragma once

#include <array>
#include <string>

enum class SpecialKey {
  Backspace,
  Enter,
  Escape,
  ArrowDown,
  ArrowLeft,
  ArrowRight,
  ArrowUp,
  Unhandled,
  Count,
};

constexpr std::array<std::string, static_cast<size_t>( SpecialKey::Count )>
  SpecialKeyNames{
    "Backspace", "Enter",      "Escape",  "ArrowDown",
    "ArrowLeft", "ArrowRight", "ArrowUp", "Unhandled",
  };
