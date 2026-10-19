#include "PlaybackEngine.h"
#include "IO.h"

#include <cmath>

PlaybackEngine::PlaybackEngine( const PadManager & pad_manager,
                                VoiceManager & voice_manager )
    : _pad_manager( pad_manager ), _voice_manager( voice_manager ) {}

void PlaybackEngine::add_note( double startBeat, double durationBeats,
                               bool is_one_shot, std::size_t padId ) {
  if ( _is_playing ) {
    IO::print_error( "Can't add note to sequence while playing" );
    return;
  }

  if ( durationBeats <= 0 ) {
    IO::print_error( "Note duration must be positive" );
    return;
  }

  // create 2 events with the same note id, 1 start and 1 end
  _sequence.add_event( { .noteId = _next_new_note_id,
                         .beat = startBeat,
                         .padId = padId,
                         .type = SequenceEvent::Type::Start } );

  if ( !is_one_shot ) {
    _sequence.add_event( { .noteId = _next_new_note_id,
                           .beat = startBeat + durationBeats,
                           .padId = padId,
                           .type = SequenceEvent::Type::Stop } );
  }

  _next_new_note_id++;
}

void PlaybackEngine::play() {
  _is_playing = true;
  _current_sample = 0;
  _next_event_index = 0;
}

void PlaybackEngine::stop() {
  _is_playing = false;
  _current_sample = 0;
  _next_event_index = 0;
  _voice_manager.stop_all();
}

void PlaybackEngine::set_sample_rate( double sample_rate ) {
  _sample_rate = sample_rate;
}

void PlaybackEngine::render( juce::AudioBuffer<float> & output ) {
  output.clear();

  if ( !_is_playing || _sample_rate <= 0.0 ) {
    _voice_manager.render( output, 0, output.getNumSamples() );
    return;
  }

  // end of this block to render
  const std::int64_t block_end = _current_sample + output.getNumSamples();
  int render_start = 0;

  // iterate through events and render
  while ( _next_event_index < _sequence.size() ) {
    const SequenceEvent & event =
      _sequence.get_event_at_index( _next_event_index );

    // convert beat to sample number
    const std::int64_t event_sample = static_cast<std::int64_t>(
      std::llround( event.beat * 60.0 * _sample_rate / _tempo_bpm ) );

    // stop if event starts after block end
    if ( event_sample >= block_end ) {
      break;
    }

    // skip event if already passed somehow
    if ( event_sample < _current_sample ) {
      ++_next_event_index;
      continue;
    }

    // render up to event start (there might be some space before next event)
    const int event_offset = static_cast<int>( event_sample - _current_sample );
    _voice_manager.render( output, render_start, event_offset - render_start );

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

    render_start = event_offset;
    _next_event_index++;
  }

  _voice_manager.render( output, render_start,
                         output.getNumSamples() - render_start );
  _current_sample = block_end;
}
