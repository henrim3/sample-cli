#pragma once

#include "Pad.h"
#include "Voice.h"

#include <cstddef>
#include <vector>

class Sample;

class VoiceManager {
public:
  VoiceManager();

  void create_voice_for_sample( const Sample & sample );
  void create_voice_for_pad( const Pad & pad );

  void stop_voices_for_sample( std::size_t sample_id );
  void stop_voices_for_pad( std::size_t pad_id );

  void stop_all();

  void render( juce::AudioBuffer<float> & output );

private:
  std::vector<Voice> _voices = std::vector<Voice>();

  Voice * get_available_voice();
};
