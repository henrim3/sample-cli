#include "FormatManager.h"

#include "juce_audio_formats/juce_audio_formats.h"

FormatManager::FormatManager() {
  _format_manager.registerBasicFormats();
}

juce::AudioFormatManager & FormatManager::get_format_manager() {
  return _format_manager;
}
