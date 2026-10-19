#include "SequenceEvent.h"
#include <vector>

class Sequence {
public:
  void add_event( const SequenceEvent & event );
  const SequenceEvent & get_event_at_index( std::size_t i ) const;

  std::size_t size() const;

private:
  std::vector<SequenceEvent> _events;

  std::size_t get_index_to_insert_at( double startBeat ) const;
};
