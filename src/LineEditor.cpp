#include "LineEditor.h"

void LineEditor::insert( std::string_view s ) {
  // handle end-of-line input
  if ( _cursor_pos == _buffer.size() ) {
    _buffer += s;
    _cursor_pos++;
    return;
  }

  // mid-line input
  _buffer.insert( _cursor_pos, s );
  _cursor_pos++;
}

bool LineEditor::backspace() {
  if ( _buffer.empty() || _cursor_pos == 0 ) {
    return false;
  }

  _buffer.erase( _cursor_pos - 1, 1 );
  _cursor_pos--;
  return true;
}

void LineEditor::enter() {
  _buffer = "";
  _cursor_pos = 0;
}

bool LineEditor::move_left() {
  if ( _cursor_pos == 0 ) {
    return false;
  }

  _cursor_pos--;
  return true;
}
bool LineEditor::move_right() {
  if ( _cursor_pos >= _buffer.size() ) {
    return false;
  }

  _cursor_pos++;
  return true;
}

void LineEditor::set_text( std::string_view s ) {
  _buffer = std::string( s );
}

std::string_view LineEditor::get_text() const {
  return _buffer;
}

std::size_t LineEditor::get_cursor_pos() const {
  return _cursor_pos;
}
