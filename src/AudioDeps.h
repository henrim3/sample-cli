#pragma once

#include "juce_audio_formats/juce_audio_formats.h"

class AudioDeps {
public:
  AudioDeps();

  juce::AudioFormatManager &get_format_manager();

private:
  juce::AudioFormatManager _format_manager;
};
