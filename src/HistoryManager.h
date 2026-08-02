#pragma once

#include "TypeDefs.h"
#include <string>
#include <vector>

class HistoryManager {
public:
  void add_entry( std::string_view s );

  MaybeStringView older_command();
  MaybeStringView newer_command();
  void reset_cursor();
  const std::vector<std::string> & get_entries() const;

private:
  // Entries are stored from oldest to newest
  std::vector<std::string> _entries;
  // Cursor points to the last returned entry
  std::size_t _cursor_pos = 0;
};
