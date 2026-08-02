#pragma once

#include "TypeDefs.h"
#include <string>
#include <vector>

class HistoryManager {
public:
  void add_entry( std::string_view s );

  MaybeStringView previous_command();
  MaybeStringView next_command();
  void reset_cursor();
  const std::vector<std::string> & get_entries() const;

private:
  // Entries are stored from oldest to newest
  std::vector<std::string> _entries;
  // Cursor is by default at the back of _entries
  std::size_t _cursor_pos = 0;
};
