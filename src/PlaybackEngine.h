#include "PadManager.h"
#include "Sequence.h"
#include "VoiceManager.h"
#include <cstddef>
#include <cstdint>

class PlaybackEngine {
public:
  PlaybackEngine( const PadManager & pad_manager,
                  VoiceManager & voice_manager );

  void add_note( double startBeat, double durationbeats, bool is_one_shot,
                 std::size_t padId );
  void play();
  void stop();
  void set_sample_rate( double sample_rate );
  void render( juce::AudioBuffer<float> & output );

private:
  const PadManager & _pad_manager;
  VoiceManager & _voice_manager;

  std::size_t _next_new_note_id = 0;

  Sequence _sequence;
  bool _is_playing = false;
  double _tempo_bpm = 88;
  std::int64_t _current_sample = 0;
  double _sample_rate = 0.0;
  std::size_t _next_event_index = 0;
};
