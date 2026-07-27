#pragma once

#include "AudioEngine.h"
#include "juce_audio_devices/juce_audio_devices.h"
class DeviceManager {
public:
  DeviceManager(AudioEngine &audio_engine);
  ~DeviceManager();

  juce::AudioDeviceManager &get_device_manager();

private:
  juce::AudioDeviceManager _device_manager;
  AudioEngine &_audio_engine;
};
