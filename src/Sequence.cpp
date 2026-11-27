#include "Sequence.h"

Sequence::Sequence( double length_beats ) : _length_beats( length_beats ) {}

void Sequence::add_event( const SequenceEvent & event ) {
  std::size_t index = get_index_to_insert_at( event.beat );
  _events.insert( _events.begin() + static_cast<long>( index ), event );
}

std::size_t Sequence::get_index_to_insert_at( double beat ) const {
  // eventually do a binary search
  for ( std::size_t i = 0; i < _events.size(); i++ ) {
    if ( _events[i].beat > beat ) {
      return i;
    }
  }

  return _events.size();
}

const SequenceEvent & Sequence::get_event_at_index( std::size_t i ) const {
  return _events[i];
}

void Sequence::set_length( double length_beats ) {
  _length_beats = length_beats;
}

double Sequence::get_length() const {
  return _length_beats;
}

std::size_t Sequence::num_events() const {
  return _events.size();
}
