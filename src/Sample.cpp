#include "Sample.h"
#include <juce_audio_formats/juce_audio_formats.h>

#include "AudioDeps.h"
#include "juce_audio_basics/juce_audio_basics.h"

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

  _buffer.setSize(static_cast<int>(reader->numChannels),
                  static_cast<int>(reader->lengthInSamples));

  if (!reader->read(&_buffer, 0, _buffer.getNumSamples(), 0, true, true)) {
    return false;
  }

  _sample_rate = reader->sampleRate;
  _length_in_samples = reader->lengthInSamples;
  _file_path = path;

  return true;
}

std::string Sample::to_string() const {
  return "Sample\nid: " + std::to_string(_id) + "\nfile path: " + _file_path +
         "\nsample rate: " + std::to_string(_sample_rate) +
         "\nlength in samples: " + std::to_string(_length_in_samples);
}

std::ostream &operator<<(std::ostream &os, Sample s) {
  return os << s.to_string();
}
