#pragma once

#include "GraphicsBuffer.h"
#include <string>

using UnicodeBuffer = GraphicsBuffer<std::string>;

using PixelBuffer = GraphicsBuffer<bool>;

class DotDisplay {
public:
  // in chars (should be terminal dimensions)
  void set_output_buffer_dimensions( std::size_t width, std::size_t height );

  void set_graphics_buffer_dimensions( std::size_t width, std::size_t height );

  void clear();

  void draw_square( std::size_t x1, std::size_t y1, std::size_t x2,
                    std::size_t y2 );

  void output();

private:
  UnicodeBuffer _output_buffer;

  // each pixel is a braille dot
  PixelBuffer _intermediate_buffer;

  // internal graphics representation, full resolution
  PixelBuffer _graphics_buffer;

  bool summarize_graphics_chunk( double x, double y, double d_x, double d_y );

  // coords are intermediate buffer coords
  std::string intermediate_chunk_to_braille( std::size_t x, std::size_t y );
};
