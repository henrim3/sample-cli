#include "ApplicationState.h"

#include <iostream>
#include <string>

ApplicationState::ApplicationState()
    : _mode(ApplicationMode::Main), _selectedSampleId(0) {}

void ApplicationState::set_mode(ApplicationMode mode) { _mode = mode; }

ApplicationMode ApplicationState::get_mode() const { return _mode; }

void ApplicationState::set_selected_sample_id(int id) {
  _selectedSampleId = id;
}

std::string ApplicationState::to_string() const {
    return "ApplicationState\n"
           "Mode: " + std::string(mode_to_string(_mode)) +
           "\nSelected sample id: " + std::to_string(_selectedSampleId);
}

std::ostream &operator<<(std::ostream &os, ApplicationState s) {
  return os << s.to_string();
}

std::string_view ApplicationState::mode_to_string(ApplicationMode mode) {
  switch (mode) {
  case ApplicationMode::Main:
    return "Main";
  case ApplicationMode::Sample:
    return "Sample";
  }
  return "Unknown";
}
