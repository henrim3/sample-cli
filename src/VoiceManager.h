#pragma once

#include <cstddef>
#include <vector>

#include "Voice.h"

class Sample;

class VoiceManager {
public:
  void create_voice( std::size_t pad_id, const Sample & sample );
  void stop_voices_for_pad( std::size_t pad_id );
  void stop_all();

  void render( juce::AudioBuffer<float> & output );

private:
  std::vector<Voice> _voices = std::vector<Voice>( 32 );
};
