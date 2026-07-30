#include "Application.h"
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

std::ostream & operator<<( std::ostream & os, Application a ) {
  return os << a._state;
}
