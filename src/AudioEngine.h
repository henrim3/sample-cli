#pragma once

class VoiceManager;

#include <juce_audio_devices/audio_io/juce_AudioIODevice.h>

class AudioEngine : public juce::AudioIODeviceCallback {
public:
  AudioEngine(VoiceManager &voice_manager);

  void audioDeviceIOCallbackWithContext(
      const float *const *input_channel_data, int num_input_channels,
      float *const *output_channel_data, int num_output_channels,
      int num_samples, const juce::AudioIODeviceCallbackContext &context);

private:
  VoiceManager &_voice_manager;
};
