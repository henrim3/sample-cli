#pragma once

#include <optional>
#include <string_view>
enum class AppMode {
  Main,
  Project,
  Pad,
  Sample,
  Sequence,
};

constexpr std::string_view to_string( AppMode mode ) {
  switch ( mode ) {
    case AppMode::Main:
      return "Main";
    case AppMode::Project:
      return "Project";
    case AppMode::Pad:
      return "Pad";
    case AppMode::Sample:
      return "Sample";
    case AppMode::Sequence:
      return "Sequence";
  }
}

using MaybeAppModeType = std::optional<AppMode>;
