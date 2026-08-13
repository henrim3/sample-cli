#include "Pad.h"

Pad::Pad( std::size_t id ) : _id( id ) {}

void Pad::set_sample( const Sample * sample ) {
  _sample = sample;
}

void Pad::trigger() const {}

std::size_t Pad::get_id() const {
  return _id;
}

const Sample * Pad::get_sample() const {
  return _sample;
}
