#include "VoiceManager.h"

#include "Sample.h"
#include "Voice.h"

void VoiceManager::create_voice( std::size_t pad_id, const Sample & sample ) {
  for ( Voice & voice : _voices ) {
    if ( !voice.is_active() ) {
      voice.start( pad_id, sample );
      return;
    }
  }

  std::cout << "Ran out of voices!" << std::endl;
}

void VoiceManager::stop_voices_for_pad( std::size_t pad_id ) {
  for ( Voice & voice : _voices ) {
    if ( voice.is_active() && voice.get_pad_id() == pad_id ) {
      voice.stop();
    }
  }
}

void VoiceManager::render( juce::AudioBuffer<float> & output ) {
  output.clear();

  for ( Voice & voice : _voices ) {
    voice.render( output, 0, output.getNumSamples() );
  }
}
