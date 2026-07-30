#include "SampleManager.h"
#include "FormatManager.h"
#include "Output.h"
#include "Sample.h"
#include <stdexcept>

SampleManager::SampleManager( FormatManager & format_manager )
    : _format_manager( format_manager ) {}

Sample * SampleManager::load_sample( std::string_view file_path ) {
  std::size_t id = _next_id++;
  Sample sample( id );

  juce::File file(( std::string( file_path ) ));

  if ( !file.existsAsFile() ) {
    Output::error( "File ", file_path, "doesn't exist or is a directory" );
    return nullptr;
  }

  auto reader = std::unique_ptr<juce::AudioFormatReader>(
    _format_manager.get_format_manager().createReaderFor( file ) );

  if ( !reader ) {
    Output::error( "No registered formats can open file ", file_path );
    return nullptr;
  }

  sample._buffer.setSize( static_cast<int>( reader->numChannels ),
                          static_cast<int>( reader->lengthInSamples ) );

  if ( !reader->read( &sample._buffer, 0, sample._buffer.getNumSamples(), 0,
                      true, true ) ) {
    Output::error( "Coudln't read file ", file_path );
    return nullptr;
  }

  sample._sample_rate = reader->sampleRate;
  sample._length_in_samples = reader->lengthInSamples;
  sample._file_path = file_path;

  _samples.insert( { id, sample } );

  return &_samples.at( id );
}

Sample * SampleManager::get_sample_by_id( std::size_t id ) {
  try {
    return &_samples.at( id );
  } catch ( std::out_of_range & ) {
    return nullptr;
  }
}

bool SampleManager::has_sample( std::size_t id ) const {
  return _samples.contains( id );
}

std::size_t SampleManager::num_samples() const {
  return _samples.size();
}
