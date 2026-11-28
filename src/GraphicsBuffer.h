#pragma once

#include "IO.h"
#include <vector>
template <typename T>
class GraphicsBuffer {
public:
  GraphicsBuffer( std::size_t width, std::size_t height, const T & default_val )
      : _default_val( default_val ), _width( width ), _height( height ) {
    _buffer = std::vector<std::vector<T>>(
      height, std::vector<T>( width, default_val ) );
  }

  std::size_t get_width() const {
    return _width;
  }
  std::size_t get_height() const {
    return _height;
  }

  const T & get( std::size_t x, std::size_t y ) const {
    check_bounds( x, y );
    return _buffer[y][x];
  }

  void set( std::size_t x, std::size_t y, T val ) {
    check_bounds( x, y );
    _buffer[y][x] = val;
  }

  void set_all( const T & val ) {
    for ( std::size_t r = 0; r < _height; r++ ) {
      for ( std::size_t c = 0; c < _width; c++ ) {
        set( c, r, val );
      }
    }
  }

  void clear() {
    set_all( _default_val );
  }

private:
  T _default_val;
  std::size_t _width;
  std::size_t _height;
  std::vector<std::vector<T>> _buffer;

  void check_bounds( std::size_t x, std::size_t y ) const {
    if ( x >= _width || y >= _height ) {
      IO::print_error( "Attempting to access value outside of buffer" );
    }
  }
};
