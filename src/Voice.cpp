#include "Voice.h"

#include "Sample.h"
#include <optional>

Voice::Voice( std::size_t id ) : _id( id ) {}

std::size_t Voice::get_id() const {
  return _id;
}

MaybeSizeT Voice::get_sample_id() const {
  if ( _sample == nullptr ) {
    return std::nullopt;
  }
  return _sample->get_id();
}

MaybeSizeT Voice::get_pad_id() const {
  return _pad_id;
}

bool Voice::is_active() const {
  return _is_active;
}

void Voice::start( const Sample & sample ) {
  _sample = &sample;
  _position = 0;
  _is_active = true;
}

void Voice::start( std::size_t pad_id, const Sample & sample ) {
  _pad_id = pad_id;
  _sample = &sample;
  _position = 0;
  _is_active = true;
}

void Voice::render( juce::AudioBuffer<float> & output, int start_sample,
                    int num_samples ) {
  if ( !_is_active ) {
    return;
  }

  auto & src = _sample->get_buffer();

  for ( int i = 0; i < num_samples; i++ ) {
    // stop playing if reached end of sample
    if ( _position >= src.getNumSamples() ) {
      _is_active = false;
      break;
    }

    float left = src.getSample( 0, _position );
    output.addSample( 0, start_sample + i, left );

    // add right channel if stereo
    if ( output.getNumChannels() > 1 ) {
      float right =
        src.getSample( std::min( 1, src.getNumChannels() - 1 ), _position );
      output.addSample( 1, start_sample + i, right );
      ;
    }

    _position++;
  }
}

void Voice::stop() {
  _sample = nullptr;
  _is_active = false;
}
