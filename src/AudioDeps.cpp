#include "AudioDeps.h"

#include "AudioEngine.h"
#include "juce_audio_formats/juce_audio_formats.h"

AudioDeps::AudioDeps(AudioEngine &audio_engine) {
  _format_manager.registerBasicFormats();
  _device_manager.initialiseWithDefaultDevices(0, 2);
  _device_manager.addAudioCallback(&audio_engine);
}

juce::AudioFormatManager &AudioDeps::get_format_manager() {
  return _format_manager;
}

juce::AudioDeviceManager &AudioDeps::get_device_manager() {
  return _device_manager;
}
