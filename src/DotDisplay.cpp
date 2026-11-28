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

  _intermediate_buffer = PixelBuffer( _output_buffer.get_width() * 2,
                                      _output_buffer.get_height() * 3, false );

  double d_y = static_cast<double>( _graphics_buffer.get_height() ) /
               ( _output_buffer.get_height() * 3 );
  double d_x = static_cast<double>( _graphics_buffer.get_width() ) /
               ( _output_buffer.get_width() * 2 );


  // g for graphics
  for ( std::size_t i_y = 0; i_y < _intermediate_buffer.get_height(); i_y++ ) {
    for ( std::size_t i_x = 0; i_x < _intermediate_buffer.get_width(); i_x++ ) {
      _intermediate_buffer.set(
        i_x, i_y, summarize_graphics_chunk( i_x * d_x, i_y * d_y, d_x, d_y ) );
    }
  }

  // convert intermediate buffer to braille characters in output buffer
  // o for output
  for ( std::size_t o_y = 0; o_y < _output_buffer.get_height(); o_y++ ) {
    for ( std::size_t o_x = 0; o_x < _output_buffer.get_width(); o_x++ ) {
      _output_buffer.set( o_x, o_y, intermediate_chunk_to_braille( o_x, o_y ) );
    }
  }

  // time to output
  IO::clear_screen();

  for ( std::size_t o_y = 0; o_y < _output_buffer.get_height(); o_y++ ) {
    for ( std::size_t o_x = 0; o_x < _output_buffer.get_width(); o_x++ ) {
      IO::print_no_flush( _output_buffer.get( o_x, o_y ) );
    }
    IO::print_no_flush( "\n" );
  }

  IO::flush_output();
}

bool DotDisplay::summarize_graphics_chunk( double x, double y, double d_x,
                                           double d_y ) {
  // I made this math up myself

  double numerator = 0;
  double denominator = 0;

  // TODO: factor in the before remainder since it can start off the grid
  // for now just skip that part

  std::size_t x_ceil = std::ceil( x );
  std::size_t y_ceil = std::ceil( y );

  // TODO: remove this temporary fix
  if ( x_ceil >= _graphics_buffer.get_width() || y_ceil >= _graphics_buffer.get_height() ) {
    return false;
  }

  // do the integer part first
  for ( std::size_t g_y = y_ceil; g_y < y + d_y; g_y++ ) {
    for ( std::size_t g_x = x_ceil; g_x < x + d_x; g_x++ ) {
      if ( _graphics_buffer.get( g_x, g_y ) ) {
        numerator++;
      }
      denominator++;
    }
  }

  // WARNING: this could be breaking things
  bool x_end_is_int = is_int( x + d_x );
  bool y_end_is_int = is_int( y + d_y );

  // if no remainders we're done
  if ( x_end_is_int && y_end_is_int ) {
    return ( numerator / denominator ) > 0.5;
  }

  // factor in remainders proportionally
  // r means remainder
  double r_x = std::fmod( d_x, 1.0 );
  double r_y = std::fmod( d_y, 1.0 );

  // q for quotient
  std::size_t q_x = std::trunc( d_x );
  std::size_t q_y = std::trunc( d_y );

  if ( !x_end_is_int ) {
    for ( std::size_t yy = y; yy < q_y; yy++ ) {
      if ( _graphics_buffer.get( q_x, yy ) ) {
        numerator += r_x;
      }
      denominator += r_x;
    }
  }

  if ( !y_end_is_int ) {
    for ( std::size_t xx = x; xx < q_x; xx++ ) {
      if ( _graphics_buffer.get( q_y, xx ) ) {
        numerator += r_y;
      }
      denominator += r_y;
    }

    // if both do special thing for corner pixel
    if ( !x_end_is_int && _graphics_buffer.get( q_x, q_y ) ) {
      numerator += r_x * r_y;
      denominator += r_x * r_y;
    }
  }

  return ( numerator / denominator ) > 0.5;
}

std::string DotDisplay::intermediate_chunk_to_braille( std::size_t x,
                                                       std::size_t y ) {
  // watch this
  std::size_t thing = 0;

  // loop through each braille dot in chunk
  for ( std::size_t i_y = y * 3; i_y < y * 3 + 3; i_y++ ) {
    for ( std::size_t i_x = x * 2; i_x < x * 2 + 2; i_x++ ) {
      thing <<= 1;
      thing += _intermediate_buffer.get( i_x, i_y );
    }
  }

  return braille.at( thing );
}
