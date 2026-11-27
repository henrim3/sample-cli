#pragma once

#include "PadManager.h"
#include "Sequence.h"
#include "VoiceManager.h"
#include <cstddef>
#include <cstdint>

class PlaybackEngine {
public:
  PlaybackEngine( const PadManager & pad_manager,
                  VoiceManager & voice_manager );

  void add_note( double start_beat, double duration_beats, bool is_one_shot,
                 std::size_t pad_id );
  void set_sequence_length( double length_beats );

  void play();
  void stop();

  void set_sample_rate( double sample_rate );

  void render( juce::AudioBuffer<float> & output );

private:
  const PadManager & _pad_manager;
  VoiceManager & _voice_manager;

  std::size_t _next_new_note_id = 0;

  Sequence _sequence{ 16 };
  bool _is_playing = false;

  bool _loop = true;

  double _tempo_bpm = 88; // TODO: move to Sequence

  // playhead in samples
  std::int64_t _playhead_sample = 0;

  double _sample_rate = 0.0;
  std::size_t _next_event_index = 0;

  const std::int64_t beats_to_samples( double beats ) const;
};
