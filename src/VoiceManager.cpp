#include "VoiceManager.h"

#include "IO.h"
#include "Sample.h"
#include "Voice.h"

VoiceManager::VoiceManager() {
  for ( std::size_t i = 0; i < 32; i++ ) {
    _voices.push_back( Voice( i ) );
  }
}

void VoiceManager::create_voice_for_sample( const Sample & sample ) {
  Voice * voice = get_available_voice();
  if ( voice == nullptr ) {
    IO::print_error( "Ran out of voices!" );
    return;
  }

  voice->start( sample );
  IO::print_debug( "Started playing voice ", voice->get_id(), " for sample ",
                   sample.get_id() );
}

void VoiceManager::create_voice_for_pad( const Pad & pad ) {
  Voice * voice = get_available_voice();
  if ( voice == nullptr ) {
    IO::print_error( "Ran out of voices!" );
    return;
  }

  const Sample * sample = pad.get_sample();

  voice->start( pad.get_id(), *sample );
  IO::print_debug( "Started playing voice ", voice->get_id(), " for pad ",
                   pad.get_id(), " sample ", sample->get_id() );
}

void VoiceManager::stop_voices_for_sample( std::size_t sample_id ) {
  for ( Voice & voice : _voices ) {
    if ( voice.get_sample_id() == sample_id && voice.is_active() ) {
      voice.stop();
    }
  }
}

void VoiceManager::stop_voices_for_pad( std::size_t pad_id ) {
  for ( Voice & voice : _voices ) {
    if ( voice.is_active() && voice.get_pad_id() == pad_id ) {
      voice.stop();
    }
  }
}

void VoiceManager::stop_all() {
  for ( Voice & voice : _voices ) {
    if ( voice.is_active() ) {
      voice.stop();
    }
  }
}

void VoiceManager::render( juce::AudioBuffer<float> & output ) {
  output.clear();

  for ( Voice & voice : _voices ) {
    if ( voice.is_active() ) {
      voice.render( output, 0, output.getNumSamples() );
    }
  }
}

Voice * VoiceManager::get_available_voice() {
  for ( std::size_t i = 0; i < _voices.size(); i++ ) {
    Voice & voice = _voices[i];
    if ( !voice.is_active() ) {
      return &voice;
    }
  }
  return nullptr;
}
