#pragma once

#include "AppModeType.h"
#include <iostream>
#include <string>

class AppState {
public:
  AppState( AppModeType mode_type );

  AppModeType get_mode_type() const;
  void set_mode_type( AppModeType mode_type );

  void select_sample( std::size_t id );
  bool select_last_sample();
  std::size_t get_selected_sample_id() const;

  void deselect_sample();

  std::string to_string() const;

  friend std::ostream & operator<<( std::ostream & os, AppState s );

private:
  AppModeType _mode_type;
  bool _is_sample_selected;
  std::size_t _selected_sample_id;
};
