#pragma once

#include "Sample.h"
#include <cstddef>

// Owns configuration that the user interacts with.
//
// Ex: sample, gain, pan, pitch, looping
class Pad {
public:
  Pad(std::size_t id);

  void set_sample(const Sample *sample);
  void trigger() const;
  
private:
  std::size_t _id;
  const Sample *_sample;
  float gain = 1.0f;
  bool loop = false;
};
