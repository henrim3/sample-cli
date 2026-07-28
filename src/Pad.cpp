#include "Pad.h"

Pad::Pad( std::size_t id ) : _id( id ) {}

void Pad::set_sample( const Sample * sample ) {
  _sample = sample;
}

void Pad::trigger() const {}
