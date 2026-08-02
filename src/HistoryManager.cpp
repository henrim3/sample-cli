#include "HistoryManager.h"

void HistoryManager::add_entry( std::string_view s ) {
  _entries.push_back( std::string( s ) );
}

std::string_view HistoryManager::get_entry( std::size_t i ) const {
  return _entries[i];
}

std::size_t HistoryManager::num_entries() const {
  return _entries.size();
}
