#pragma once

#include <string>
#include <string_view>

// Maintains the editor state, shouldn't call IO directly
class LineEditor {
public:
  void insert( std::string_view s );
  bool backspace();
  void enter();

  bool move_left();
  bool move_right();

  void set_text( std::string_view s );
  std::string_view get_text() const;

  std::size_t get_cursor_pos() const;

private:
  std::string _buffer = "";
  std::size_t _cursor_pos = 0;
  std::string _last_prompt = "";
};
