#pragma once

#include "TypeDefs.h"
#include "juce_audio_basics/juce_audio_basics.h"

class Sample;

class Voice {
public:
  Voice( std::size_t id );

  std::size_t get_id() const;
  MaybeSizeT get_sample_id() const;
  MaybeSizeT get_pad_id() const;
  bool is_active() const;

  void start( const Sample & sample );
  void start( std::size_t pad_id, const Sample & sample );
  void stop();

  void render( juce::AudioBuffer<float> & output, int start_sample,
               int num_samples );

private:
  std::size_t _id;
  MaybeSizeT _pad_id;
  const Sample * _sample = nullptr;
  int _position = 0;
  bool _is_active = false;
};
