#include "Math.h"
#include <cmath>

bool is_int( double x ) {
  return std::isfinite( x ) && std::trunc( x ) == x;
}
