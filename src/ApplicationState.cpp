#include "ApplicationState.h"

#include <iostream>
#include <string>

ApplicationState::ApplicationState(AudioDeps &audio_deps)
    : _mode(ApplicationMode::Main), _did_select_sample(false),
      _selected_sample_id(0), _audio_deps(audio_deps) {}

void ApplicationState::set_mode(ApplicationMode mode) { _mode = mode; }

ApplicationMode ApplicationState::get_mode() const { return _mode; }

bool ApplicationState::select_sample(size_t id) {
  if (id >= _samples.size()) {
    return false;
  }

  _did_select_sample = true;
  _mode = ApplicationMode::Sample;
  _selected_sample_id = id;

  return true;
}

bool ApplicationState::select_last_sample() {
  if (!_did_select_sample) {
    return false;
  }

  _did_select_sample = true;
  _mode = ApplicationMode::Sample;

  return true;
}

size_t ApplicationState::get_selected_sample_id() const {
  return _selected_sample_id;
}

void ApplicationState::deselect_sample() { _mode = ApplicationMode::Main; }

Sample *ApplicationState::create_sample() {
  _samples.push_back(Sample(_samples.size(), _audio_deps));
  return &_samples.back();
}

Sample *ApplicationState::get_sample(size_t id) {
  if (id >= _samples.size()) {
    return nullptr;
  }
  return &_samples[id];
}

Sample *ApplicationState::get_current_sample() {
  return get_sample(_selected_sample_id);
}

std::string ApplicationState::to_string() const {
  std::string samples_str = "";

  for (Sample s : _samples) {
    samples_str += s.to_string() + "\n";
  }

  return "ApplicationState\n"
         "Mode: " +
         std::string(mode_to_string(_mode)) +
         "\nSelected sample id: " + std::to_string(_selected_sample_id) + "\n" +
         std::to_string(_samples.size()) + " Samples:" + samples_str;
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
