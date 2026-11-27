#include "PlaybackEngine.h"
#include "IO.h"

#include <algorithm>
#include <cmath>
#include <limits>

PlaybackEngine::PlaybackEngine( const PadManager & pad_manager,
                                VoiceManager & voice_manager )
    : _pad_manager( pad_manager ), _voice_manager( voice_manager ) {}

void PlaybackEngine::add_note( double start_beat, double duration_beats,
                               bool is_one_shot, std::size_t pad_id ) {
  if ( _is_playing ) {
    IO::print_error( "Can't add note to sequence while playing" );
    return;
  }

  if ( !std::isfinite( start_beat ) || start_beat < 0 ||
       !std::isfinite( duration_beats ) || duration_beats <= 0 ) {
    IO::print_error( "Note duration must be positive" );
    return;
  }

  double sequence_length_beats = _sequence.get_length();

  if ( start_beat >= sequence_length_beats ) {
    IO::print_error( "Note must start before ", sequence_length_beats );
    return;
  }

  if ( ( start_beat + duration_beats ) >= sequence_length_beats ) {
    IO::print_error( "Note must end before ", sequence_length_beats );
    return;
  }

  // create start event
  _sequence.add_event( { .noteId = _next_new_note_id,
                         .beat = start_beat,
                         .padId = pad_id,
                         .type = SequenceEvent::Type::Start } );

  // create end event if not one shot
  if ( !is_one_shot ) {
    _sequence.add_event( { .noteId = _next_new_note_id,
                           .beat = start_beat + duration_beats,
                           .padId = pad_id,
                           .type = SequenceEvent::Type::Stop } );
  }

  _next_new_note_id++;
}

void PlaybackEngine::set_sequence_length( double length_beats ) {
  if ( _is_playing || !std::isfinite( length_beats ) || length_beats <= 0 ) {
    IO::print_error( "Sequence length must be positive and set while stopped" );
    return;
  }
  for ( std::size_t i = 0; i < _sequence.num_events(); ++i ) {
    if ( _sequence.get_event_at_index( i ).beat >= length_beats ) {
      IO::print_error( "Sequence length must include all existing events" );
      return;
    }
  }
  _sequence.set_length( length_beats );
}

void PlaybackEngine::play() {
  _is_playing = true;
  _playhead_sample = 0;
  _next_event_index = 0;
}

void PlaybackEngine::stop() {
  _is_playing = false;
  _playhead_sample = 0;
  _next_event_index = 0;
  _voice_manager.stop_all();
}

void PlaybackEngine::set_sample_rate( double sample_rate ) {
  _sample_rate = sample_rate;
}

void PlaybackEngine::render( juce::AudioBuffer<float> & output ) {
  output.clear();

  // if not playing or no sample rate, just play active voices
  if ( !_is_playing || _sample_rate <= 0.0 ) {
    _voice_manager.render( output, 0, output.getNumSamples() );
    return;
  }

  // position in output buffer
  int output_buffer_cursor = 0;

  const double length_samples =
    _sequence.get_length() * 60.0 * _sample_rate / _tempo_bpm;
  if ( !std::isfinite( length_samples ) || length_samples < 0.5 ||
       length_samples >=
         static_cast<double>( std::numeric_limits<std::int64_t>::max() ) ) {
    _voice_manager.render( output, 0, output.getNumSamples() );
    return;
  }
  const std::int64_t sequence_end_sample =
    beats_to_samples( _sequence.get_length() );
  if ( _playhead_sample >= sequence_end_sample ) {
    _playhead_sample = 0;
    _next_event_index = 0;
  }

  while ( output_buffer_cursor < output.getNumSamples() ) {
    const int segment_length = static_cast<int>(
      std::min<std::int64_t>( output.getNumSamples() - output_buffer_cursor,
                              sequence_end_sample - _playhead_sample ) );
    const std::int64_t segment_end = _playhead_sample + segment_length;
    const int segment_start = output_buffer_cursor;

    // iterate through events and render
    while ( _next_event_index < _sequence.num_events() ) {
      const SequenceEvent & event =
        _sequence.get_event_at_index( _next_event_index );

      if ( event.beat >= _sequence.get_length() ) {
        break;
      }
      const std::int64_t event_sample =
        std::min( beats_to_samples( event.beat ), sequence_end_sample - 1 );

      // leave events at or after the segment end for the next segment
      if ( event_sample >= segment_end ) {
        break;
      }

      // skip event if already passed somehow
      if ( event_sample < _playhead_sample ) {
        _next_event_index++;
        continue;
      }

      // position of next event in output buffer
      const int event_offset =
        segment_start + static_cast<int>( event_sample - _playhead_sample );

      // render up to event start (there might be some space before next event)
      _voice_manager.render( output, output_buffer_cursor,
                             event_offset - output_buffer_cursor );

      // start or stop the voice
      switch ( event.type ) {
        case SequenceEvent::Type::Start: {
          const Pad * pad = _pad_manager.get_pad_by_id( event.padId );
          if ( pad != nullptr && pad->get_sample() != nullptr ) {
            _voice_manager.create_voice_for_note( event.noteId,
                                                  *pad->get_sample() );
          }
          break;
        }

        case SequenceEvent::Type::Stop: {
          _voice_manager.stop_voices_for_note( event.noteId );
          break;
        }
      }

      output_buffer_cursor = event_offset;
      _next_event_index++;
    }

    const int segment_buffer_end = segment_start + segment_length;
    _voice_manager.render( output, output_buffer_cursor,
                           segment_buffer_end - output_buffer_cursor );
    output_buffer_cursor = segment_buffer_end;
    _playhead_sample = segment_end;
    if ( _playhead_sample == sequence_end_sample ) {
      _playhead_sample = 0;
      _next_event_index = 0;
    }
  }
}

const std::int64_t PlaybackEngine::beats_to_samples( double beats ) const {
  return static_cast<std::int64_t>(
    std::llround( beats * 60.0 * _sample_rate / _tempo_bpm ) );
}
