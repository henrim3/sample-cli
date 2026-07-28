#pragma once

#include "Pad.h"
#include <cstddef>
#include <unordered_map>
class PadManager {
public:
  Pad & create_pad();
  Pad * get_pad_by_id( std::size_t id );

private:
  std::size_t _next_id = 0;
  std::unordered_map<std::size_t, Pad> _pads;
};
