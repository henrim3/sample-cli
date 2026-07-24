#include "juce_core/juce_core.h"
#include <juce_audio_formats/juce_audio_formats.h>

#include "AudioDeps.h"

class Sample {
public:
  Sample(size_t id, AudioDeps &deps);

  size_t get_id() const;

  std::string get_file_path() const;

  bool load_file(std::string path);

  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, Sample s);

private:
  size_t _id;
  AudioDeps &_deps;
  std::string _file_path;
  juce::File _file;
};
