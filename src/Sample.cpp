#include "Sample.h"
#include <juce_audio_formats/juce_audio_formats.h>

#include "juce_audio_basics/juce_audio_basics.h"

Sample::Sample(std::size_t id) : _id(id) {}

std::size_t Sample::get_id() const { return _id; }

std::string Sample::get_file_path() const { return _file_path; }

const juce::AudioBuffer<float> &Sample::get_buffer() const { return _buffer; }

std::string Sample::to_string() const {
  return "Sample\nid: " + std::to_string(_id) + "\nfile path: " + _file_path +
         "\nsample rate: " + std::to_string(_sample_rate) +
         "\nlength in samples: " + std::to_string(_length_in_samples);
}

std::ostream &operator<<(std::ostream &os, Sample s) {
  return os << s.to_string();
}
