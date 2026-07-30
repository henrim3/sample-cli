#include "Application.h"
#include "ApplicationState.h"
#include "Output.h"
#include "Sample.h"
#include "SampleManager.h"

Application::Application() {}

SampleManager & Application::get_sample_manager() {
  return _sample_manager;
}

ApplicationState & Application::get_state() {
  return _state;
}

Sample * Application::get_selected_sample() {
  return _sample_manager.get_sample_by_id( _state.get_selected_sample_id() );
}

bool Application::select_sample( std::size_t id ) {
  if ( !_sample_manager.has_sample( id ) ) {
    Output::error( "Sample ", id, " doesn't exist" );
    return false;
  }

  _state.select_sample( id );
  return true;
}

bool Application::load_sample( std::string_view file_path ) {
  Sample * sample = _sample_manager.load_sample( file_path );
  if ( sample == nullptr ) {
    Output::error( "Couldn't load sample ", file_path );
    return false;
  }

  Output::println( "Successfully loaded file ", file_path, " into sample ",
                   sample->get_id() );

  return true;
}

bool Application::play_current_sample() {
  Sample * sample = get_selected_sample();
  if ( sample == nullptr ) {
    Output::error( "No sample selected" );
    return false;
  }

  _voice_manager.create_voice( 0, *sample );

  return true;
}

bool Application::play() {
  switch ( _state.get_mode() ) {
    case ApplicationMode::Project:
      Output::error( "Command not supported yet!" );
      return false;

    case ApplicationMode::Sample:
      bool res = play_current_sample();
      if ( !res ) {
        Output::error( "Couldn't play current sample" );
        return false;
      }
      return true;
  }
}

std::ostream & operator<<( std::ostream & os, Application a ) {
  return os << a._state;
}
