#include "SampleManager.h"
#include "Sample.h"
#include <stdexcept>

Sample *SampleManager::load_sample(const std::string &path) {
  std::size_t id = _next_id++;
  Sample sample(id);

  juce::File file(path);
  auto reader = std::unique_ptr<juce::AudioFormatReader>(
      _format_manager.get_format_manager().createReaderFor(file));

  if (!reader) {
    return nullptr;
  }

  sample._buffer.setSize(static_cast<int>(reader->numChannels),
                         static_cast<int>(reader->lengthInSamples));

  if (!reader->read(&sample._buffer, 0, sample._buffer.getNumSamples(), 0, true,
                    true)) {
    return nullptr;
  }

  sample._sample_rate = reader->sampleRate;
  sample._length_in_samples = reader->lengthInSamples;
  sample._file_path = path;

  _samples.insert({id, sample});

  return &_samples.at(id);
}

Sample *SampleManager::get_sample_by_id(std::size_t id) {
  try {
    return &_samples.at(id);
  } catch (std::out_of_range &) {
    return nullptr;
  }
}
