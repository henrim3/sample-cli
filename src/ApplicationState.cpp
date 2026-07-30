#include "ApplicationState.h"

#include <iostream>
#include <string>

ApplicationState::ApplicationState()
    : _mode( ApplicationMode::Project ), _is_sample_selected( false ),
      _selected_sample_id( 0 ) {}

void ApplicationState::set_mode( ApplicationMode mode ) {
  _mode = mode;
}

ApplicationMode ApplicationState::get_mode() const {
  return _mode;
}

void ApplicationState::select_sample( std::size_t id ) {
  _is_sample_selected = true;
  _mode = ApplicationMode::Sample;
  _selected_sample_id = id;
}

bool ApplicationState::select_last_sample() {
  if ( !_is_sample_selected ) {
    return false;
  }

  _is_sample_selected = true;
  _mode = ApplicationMode::Sample;

  return true;
}

std::size_t ApplicationState::get_selected_sample_id() const {
  return _selected_sample_id;
}

void ApplicationState::deselect_sample() {
  _is_sample_selected = false;
  _mode = ApplicationMode::Project;
}

std::string ApplicationState::to_string() const {

  return "ApplicationState\n"
         "Mode: " +
         std::string( mode_to_string( _mode ) ) +
         "\nSelected sample id: " + std::to_string( _selected_sample_id );
}

std::ostream & operator<<( std::ostream & os, ApplicationState s ) {
  return os << s.to_string();
}

std::string_view ApplicationState::mode_to_string( ApplicationMode mode ) {
  switch ( mode ) {
    case ApplicationMode::Project:
      return "Main";
    case ApplicationMode::Sample:
      return "Sample";
  }
  return "Unknown";
}
