#pragma once

class VoiceManager;

#include <juce_audio_devices/juce_audio_devices.h>

class AudioEngine : public juce::AudioIODeviceCallback {
public:
  AudioEngine(VoiceManager &voice_manager);

  void audioDeviceIOCallbackWithContext(
      const float *const *input_channel_data, int num_input_channels,
      float *const *output_channel_data, int num_output_channels,
      int num_samples,
      const juce::AudioIODeviceCallbackContext &context) override;

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;

    void audioDeviceStopped() override;

private:
  VoiceManager &_voice_manager;
  double _sample_rate = 0.0;
  int _block_size = 0;
};
