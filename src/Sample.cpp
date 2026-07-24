#include "Sample.h"
#include <juce_audio_formats/juce_audio_formats.h>

#include "AudioDeps.h"

Sample::Sample(size_t id, AudioDeps &deps) : _id(id), _deps(deps) {}

size_t Sample::get_id() const { return _id; }

std::string Sample::get_file_path() const { return _file_path; }

bool Sample::load_file(std::string path) {
  juce::File file(path);
  auto reader = std::unique_ptr<juce::AudioFormatReader>(
      _deps.get_format_manager().createReaderFor(file));

  if (!reader) {
    return false;
  }

  return true;
}

std::string Sample::to_string() const {
  return "Sample\nid: " + std::to_string(_id) + "\nfile path: " + _file_path;
}

std::ostream &operator<<(std::ostream &os, Sample s) {
  return os << s.to_string();
}
