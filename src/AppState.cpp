#include "AppState.h"

#include <iostream>
#include <string>

AppState::AppState( AppModeType mode_type )
    : _mode_type( mode_type ), _is_sample_selected( false ),
      _selected_sample_id( 0 ) {}

AppModeType AppState::get_mode_type() const {
  return _mode_type;
}

void AppState::set_mode_type( AppModeType mode_type ) {
  _mode_type = mode_type;
}

void AppState::select_sample( std::size_t id ) {
  _is_sample_selected = true;
  _selected_sample_id = id;
}

bool AppState::select_last_sample() {
  if ( !_is_sample_selected ) {
    return false;
  }

  _is_sample_selected = true;

  return true;
}

std::size_t AppState::get_selected_sample_id() const {
  return _selected_sample_id;
}

void AppState::deselect_sample() {
  _is_sample_selected = false;
}

std::string AppState::to_string() const {

  return "ApplicationState\nSelected sample id: " +
         std::to_string( _selected_sample_id );
}

std::ostream & operator<<( std::ostream & os, AppState s ) {
  return os << s.to_string();
}
