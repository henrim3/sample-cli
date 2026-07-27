#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include <juce_audio_formats/juce_audio_formats.h>

class FormatManager;

class Sample {
public:
  Sample(std::size_t id);

  std::size_t get_id() const;

  std::string get_file_path() const;

  const juce::AudioBuffer<float> &get_buffer() const;

  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, Sample s);

private:
  friend class SampleManager;

  std::size_t _id;
  std::string _file_path;
  juce::AudioBuffer<float> _buffer;
  double _sample_rate = 0.0;
  int64_t _length_in_samples = 0;
};
