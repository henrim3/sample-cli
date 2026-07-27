#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "FormatManager.h"
#include "Sample.h"

enum class ApplicationMode {
  Main,
  Sample,
};

class ApplicationState {
public:
  ApplicationState(FormatManager &format_manager);

  void set_mode(ApplicationMode mode);
  ApplicationMode get_mode() const;

  bool select_sample(std::size_t id);
  bool select_last_sample();
  std::size_t get_selected_sample_id() const;

  void deselect_sample();

  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, ApplicationState s);

private:
  ApplicationMode _mode;
  bool _is_sample_selected;
  std::size_t _selected_sample_id;
  std::vector<Sample> _samples;

  FormatManager &_format_manager;

  static std::string_view mode_to_string(ApplicationMode mode);
};
