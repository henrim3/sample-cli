#pragma once

class AudioEngine;

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"

class AudioDeps {
public:
  AudioDeps(AudioEngine &audio_engine);

  juce::AudioFormatManager &get_format_manager();

  juce::AudioDeviceManager &get_device_manager();

private:
  juce::AudioFormatManager _format_manager;
  juce::AudioDeviceManager _device_manager;
};
