#include "../src/PlaybackEngine.h"
#include <cmath>

int main() {
  PadManager pads;
  VoiceManager voices;
  PlaybackEngine engine( pads, voices );
  Sample sample( 0 );
  auto & data = const_cast<juce::AudioBuffer<float> &>( sample.get_buffer() );
  data.setSize( 1, 1 );
  data.setSample( 0, 0, 1.0f );
  pads.create_pad().set_sample( &sample );

  engine.set_sequence_length( 1 );
  engine.set_sample_rate( 88.0 * 8.0 / 60.0 );
  engine.add_note( 0.25, 0.25, true, 0 );
  engine.play();

  // split callbacks across a loop boundary and span several loops at once
  int position = 0;
  for ( int size : { 7, 19, 3 } ) {
    juce::AudioBuffer<float> output( 1, size );
    engine.render( output );
    for ( int i = 0; i < size; ++i, ++position ) {
      const float expected = position % 8 == 2 ? 1.0f : 0.0f;
      if ( std::abs( output.getSample( 0, i ) - expected ) > 0.0001f ) {
        return 1;
      }
    }
  }
  return 0;
}
