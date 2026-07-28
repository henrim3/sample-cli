#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

class Sample;

class Voice {
public:
  std::size_t get_pad_id() const;

  void start( std::size_t pad_id, const Sample & sample );
  void stop();

  void render( juce::AudioBuffer<float> & output, int start_sample,
               int num_samples );

  bool is_active() const;

private:
  std::size_t _pad_id;
  const Sample * _sample = nullptr;
  int _position = 0;
  bool _is_active = false;
};
