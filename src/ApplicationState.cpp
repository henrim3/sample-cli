#include "ApplicationState.h"

#include <iostream>
#include <string>

ApplicationState::ApplicationState()
    : _mode( ApplicationMode::Main ), _is_sample_selected( false ),
      _selected_sample_id( 0 ) {}

void ApplicationState::set_mode( ApplicationMode mode ) {
  _mode = mode;
}

ApplicationMode ApplicationState::get_mode() const {
  return _mode;
}

bool ApplicationState::select_sample( std::size_t id ) {
  if ( id >= _samples.size() ) {
    return false;
  }

  _is_sample_selected = true;
  _mode = ApplicationMode::Sample;
  _selected_sample_id = id;

  return true;
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
  _mode = ApplicationMode::Main;
}

std::string ApplicationState::to_string() const {
  std::string samples_str = "";

  for ( Sample s : _samples ) {
    samples_str += s.to_string() + "\n";
  }

  return "ApplicationState\n"
         "Mode: " +
         std::string( mode_to_string( _mode ) ) +
         "\nSelected sample id: " + std::to_string( _selected_sample_id ) +
         "\n" + std::to_string( _samples.size() ) + " Samples:\n" + samples_str;
}

std::ostream & operator<<( std::ostream & os, ApplicationState s ) {
  return os << s.to_string();
}

std::string_view ApplicationState::mode_to_string( ApplicationMode mode ) {
  switch ( mode ) {
    case ApplicationMode::Main:
      return "Main";
    case ApplicationMode::Sample:
      return "Sample";
  }
  return "Unknown";
}
