#pragma once

#include <optional>
enum class AppMode {
  Main,
  Project,
  Pad,
  Sample,
};

using MaybeAppModeType = std::optional<AppMode>;
