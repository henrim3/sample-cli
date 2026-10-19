#include "AudioEngine.h"
#include "PlaybackEngine.h"

#include "juce_audio_basics/juce_audio_basics.h"

AudioEngine::AudioEngine( PlaybackEngine & playback_engine )
    : _playback_engine( playback_engine ) {}

void AudioEngine::audioDeviceIOCallbackWithContext(
  const float * const * input_channel_data, int num_input_channels,
  float * const * output_channel_data, int num_output_channels, int num_samples,
  const juce::AudioIODeviceCallbackContext & context ) {
  // unused params
  (void)input_channel_data;
  (void)num_input_channels;
  (void)context;

  juce::AudioBuffer<float> output_buffer( output_channel_data,
                                          num_output_channels, num_samples );
  _playback_engine.render( output_buffer );
}

void AudioEngine::audioDeviceAboutToStart( juce::AudioIODevice * device ) {
  _sample_rate = device->getCurrentSampleRate();
  _block_size = device->getCurrentBufferSizeSamples();
  _playback_engine.set_sample_rate( _sample_rate );
}

// called when audio device stopped, reset things
void AudioEngine::audioDeviceStopped() {
  _sample_rate = 0.0;
  _block_size = 0;
  _playback_engine.set_sample_rate( 0.0 );
}
