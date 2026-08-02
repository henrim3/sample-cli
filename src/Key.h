#pragma once

#include <optional>
#include <string>

enum class SpecialKeyType {
  ArrowDown,
  ArrowLeft,
  ArrowRight,
  ArrowUp,
  Backspace,
  Enter,
  Escape,
  Unhandled,
};

using MaybeSpecialKeyType = std::optional<SpecialKeyType>;

struct Key {
  std::string s;
  MaybeSpecialKeyType special_type = std::nullopt;
};
