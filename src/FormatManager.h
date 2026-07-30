#pragma once

class AudioEngine;

#include "juce_audio_formats/juce_audio_formats.h"

class FormatManager {
public:
  FormatManager();

  juce::AudioFormatManager & get_format_manager();

private:
  juce::AudioFormatManager _format_manager;
};
