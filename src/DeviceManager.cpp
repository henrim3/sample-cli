#include "DeviceManager.h"

DeviceManager::DeviceManager(AudioEngine &audio_engine)
    : _audio_engine(audio_engine) {
  _device_manager.initialiseWithDefaultDevices(0, 2);
  _device_manager.addAudioCallback(&audio_engine);
}

DeviceManager::~DeviceManager() {
  _device_manager.removeAudioCallback(&_audio_engine);
}

juce::AudioDeviceManager &DeviceManager::get_device_manager() {
  return _device_manager;
}
