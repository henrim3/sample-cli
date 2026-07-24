#include "AudioEngine.h"
#include "VoiceManager.h"

#include "juce_audio_basics/juce_audio_basics.h"

AudioEngine::AudioEngine(VoiceManager &voice_manager)
    : _voice_manager(voice_manager) {}

void AudioEngine::audioDeviceIOCallbackWithContext(
    const float *const *input_channel_data, int num_input_channels,
    float *const *output_channel_data, int num_output_channels, int num_samples,
    const juce::AudioIODeviceCallbackContext &context) {
  // unused params
  (void)input_channel_data;
  (void)num_input_channels;
  (void)context;

  juce::AudioBuffer<float> output_buffer(output_channel_data,
                                         num_output_channels, num_samples);
  _voice_manager.render(output_buffer);
}
