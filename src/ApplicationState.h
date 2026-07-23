#pragma once

#include <iostream>
#include <string>

enum class ApplicationMode {
  Main,
  Sample,
};

class ApplicationState {
public:
  ApplicationState();

  void set_mode(ApplicationMode mode);
  ApplicationMode get_mode() const;

  void set_selected_sample_id(int id);

  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, ApplicationState);

private:
  ApplicationMode _mode;
  int _selectedSampleId;

  static std::string_view mode_to_string(ApplicationMode mode);
};
