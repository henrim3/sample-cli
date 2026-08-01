#include "VoiceManager.h"

#include "IO.h"
#include "Sample.h"
#include "Voice.h"

void VoiceManager::create_voice( std::size_t pad_id, const Sample & sample ) {
  for ( size_t i = 0; i < _voices.size(); i++ ) {
    Voice & voice = _voices[i];
    if ( !voice.is_active() ) {
      voice.start( pad_id, sample );
      IO::print_debug( "Started playing voice ", i, " for pad ", pad_id,
                       " sample ", sample.get_id() );
      return;
    }
  }

  IO::print_error( "Ran out of voices!" );
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
