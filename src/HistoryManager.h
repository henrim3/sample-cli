#pragma once

#include <string>
#include <vector>

class HistoryManager {
public:
  void add_entry( std::string_view s );
  std::string_view get_entry( std::size_t i ) const;
  std::size_t num_entries() const;

private:
  std::vector<std::string> _entries;
};
