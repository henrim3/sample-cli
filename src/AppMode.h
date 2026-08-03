#pragma once

#include <optional>
enum class AppMode {
  Main,
  Project,
  Sample,
};

using MaybeAppModeType = std::optional<AppMode>;
