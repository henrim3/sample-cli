#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "AudioDeps.h"
#include "Sample.h"

enum class ApplicationMode {
  Main,
  Sample,
};

class ApplicationState {
public:
  ApplicationState(AudioDeps &audio_deps);

  void set_mode(ApplicationMode mode);
  ApplicationMode get_mode() const;

  bool select_sample(size_t id);
  bool select_last_sample();
  size_t get_selected_sample_id() const;

  void deselect_sample();

  Sample *create_sample();
  Sample *get_sample(size_t id);
  Sample *get_current_sample();

  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, ApplicationState s);

private:
  ApplicationMode _mode;
  bool _did_select_sample;
  size_t _selected_sample_id;
  std::vector<Sample> _samples;

  AudioDeps &_audio_deps;

  static std::string_view mode_to_string(ApplicationMode mode);
};
