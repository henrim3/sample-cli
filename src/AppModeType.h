#pragma once

#include <optional>
enum class AppModeType {
  Project,
  Sample,
};

using MaybeAppModeType = std::optional<AppModeType>;
