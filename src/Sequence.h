#include "SequenceEvent.h"
#include <vector>

class Sequence {
public:
  Sequence( double length_beats );

  void add_event( const SequenceEvent & event );
  const SequenceEvent & get_event_at_index( std::size_t i ) const;

  void set_length( double length_beats );
  double get_length() const;

  std::size_t num_events() const;

private:
  std::vector<SequenceEvent> _events;
  double _length_beats;

  std::size_t get_index_to_insert_at( double startBeat ) const;
};
