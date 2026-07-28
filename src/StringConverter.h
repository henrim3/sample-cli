#pragma once

#include "TypeDefs.h"
#include <string_view>


class StringConverter {
public:
  static MaybeInt try_parse_int( std::string_view s );
  static MaybeSizeT try_parse_size_t( std::string_view s );
};
