#include "AudioDeps.h"

#include "juce_audio_formats/juce_audio_formats.h"

AudioDeps::AudioDeps() {
  _format_manager.registerBasicFormats();
}

juce::AudioFormatManager &AudioDeps::get_format_manager() {
  return _format_manager;
}
