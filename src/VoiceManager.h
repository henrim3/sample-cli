#pragma once

#include <cstddef>
#include <vector>

#include "Voice.h"

class Sample;

class VoiceManager {
public:
  void create_voice(size_t pad_id, const Sample& sample);
  void stop_voices_for_pad(size_t pad_id);

  void render(juce::AudioBuffer<float>& output);

private:
  std::vector<Voice> _voices = std::vector<Voice>(32);
};
