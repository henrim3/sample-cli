#include "FormatManager.h"

#include "juce_audio_formats/juce_audio_formats.h"

juce::AudioFormatManager & FormatManager::get_format_manager() {
  return _format_manager;
}
