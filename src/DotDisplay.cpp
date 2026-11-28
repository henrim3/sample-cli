#include "DotDisplay.h"
#include "Math.h"
#include <cmath>
#include <string>
#include <unordered_map>

const std::unordered_map<char, std::string> braille = [] {
  std::unordered_map<char, std::string> map;

  // what in the gpt
  for ( char32_t i = 0; i < 64; ++i ) {
    char32_t c = 0x2800u + ( ( i & 0b100000u ) >> 5 ) +
                 ( ( i & 0b010000u ) >> 1 ) + ( ( i & 0b001000u ) >> 2 ) +
                 ( ( i & 0b000100u ) << 2 ) + ( ( i & 0b000010u ) << 1 ) +
                 ( ( i & 0b000001u ) << 5 );

    std::string utf8 = { static_cast<char>( 0xE0 | ( c >> 12 ) ),
                         static_cast<char>( 0x80 | ( ( c >> 6 ) & 0x3F ) ),
                         static_cast<char>( 0x80 | ( c & 0x3F ) ) };

    map.emplace( static_cast<char>( i ), utf8 );
  }

  return map;
}();

void DotDisplay::set_output_buffer_dimensions( std::size_t width,
                                               std::size_t height ) {
  _output_buffer = UnicodeBuffer( width, height, "⠀" );
}

void DotDisplay::set_graphics_buffer_dimensions( std::size_t width,
                                                 std::size_t height ) {
  _graphics_buffer = PixelBuffer( width, height, false );
}

void DotDisplay::clear() {
  _graphics_buffer.clear();
}

void DotDisplay::draw_square( std::size_t x1, std::size_t y1, std::size_t x2,
                              std::size_t y2 ) {
  if ( x1 >= x2 || y1 >= y2 ) {
    IO::print_error( "Invalid square" );
    return;
  }

  for ( std::size_t y = y1; y < y2; y++ ) {
    for ( std::size_t x = x1; x < x2; x++ ) {
      _graphics_buffer.set( x, y, true );
    }
  }
}

void DotDisplay::output() {
  // first convert to an intermediate buffer where each pixel is a braille dot
  // i means intermediate
  // d means delta

  PixelBuffer i_buffer( _output_buffer.get_width() * 2,
                        _output_buffer.get_height() * 3, false );

  double d_y = static_cast<double>( _graphics_buffer.get_height() ) /
               ( _output_buffer.get_height() * 3 );
  double d_x = static_cast<double>( _graphics_buffer.get_width() ) /
               ( _output_buffer.get_width() * 2 );

  std::size_t i_x = 0;
  std::size_t i_y = 0;

  for ( double y = 0; y < _graphics_buffer.get_height(); y += d_y ) {
    for ( double x = 0; x < _graphics_buffer.get_width(); x += d_x ) {
      i_buffer.set( i_x, i_y, summarize_graphics_chunk( x, y, d_x, d_y ) );
      i_x++;
    }
    i_y++;
  }

  // convert intermediate buffer to braille characters in output buffer
}

bool DotDisplay::summarize_graphics_chunk( std::size_t x, std::size_t y,
                                           double d_x, double d_y ) {
  // I made this math up myself

  double numerator = 0;
  double denominator = 0;

  // do the integer part first
  for ( std::size_t yy = y; yy < y + d_y; yy++ ) {
    for ( std::size_t xx = x; xx < x + d_x; xx++ ) {
      if ( _graphics_buffer.get( xx, yy ) ) {
        numerator++;
      }
      denominator++;
    }
  }

  bool x_is_int = is_int( d_x );
  bool y_is_int = is_int( d_y );

  // if no remainders we're done
  if ( x_is_int && y_is_int ) {
    return ( numerator / denominator ) > 0.5;
  }

  // factor in remainders proportionally
  // r means remainder
  double r_x = std::fmod( d_x, 1.0 );
  double r_y = std::fmod( d_y, 1.0 );

  // q for quotient
  std::size_t q_x = std::trunc( d_x );
  std::size_t q_y = std::trunc( d_y );

  if ( !x_is_int ) {
    for ( std::size_t yy = y; yy < q_y; yy++ ) {
      if ( _graphics_buffer.get( q_x, yy ) ) {
        numerator += r_x;
      }
      denominator += r_x;
    }
  }

  if ( !y_is_int ) {
    for ( std::size_t xx = x; xx < q_x; xx++ ) {
      if ( _graphics_buffer.get( q_y, xx ) ) {
        numerator += r_y;
      }
      denominator += r_y;
    }

    // if both do special thing for corner pixel
    if ( !x_is_int && _graphics_buffer.get( q_x, q_y ) ) {
      numerator += r_x * r_y;
      denominator += r_x * r_y;
    }
  }

  return ( numerator / denominator ) > 0.5;
}
