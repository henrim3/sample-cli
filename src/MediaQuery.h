#pragma once

#include <cstddef>
#include <optional>

struct ScreenDimensions {
  std::size_t width;
  std::size_t height;
};

using MaybeScreenDimensions = std::optional<ScreenDimensions>;

class MediaQuery {
public:
  static MaybeScreenDimensions get_terminal_dimensions();
};
