#include "HistoryManager.h"
#include <optional>

void HistoryManager::add_entry( std::string_view s ) {
  _entries.push_back( std::string( s ) );
  _cursor_pos++;
}

MaybeStringView HistoryManager::previous_command() {
  if ( _cursor_pos == 0 || _entries.size() == 0 ) {
    return std::nullopt;
  }
  return _entries[--_cursor_pos];
}

MaybeStringView HistoryManager::next_command() {
  if ( _cursor_pos == _entries.size() - 1 || _entries.size() == 0 ) {
    return std::nullopt;
  }
  return _entries[++_cursor_pos];
}
void HistoryManager::reset_cursor() {
  _cursor_pos = _entries.size();
}

const std::vector<std::string> & HistoryManager::get_entries() const {
  return _entries;
}
