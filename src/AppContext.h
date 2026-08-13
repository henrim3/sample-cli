#pragma once

#include "AppServices.h"
#include "AppState.h"

class AppContext {
public:
  AppServices & services;
  AppState & state;

  Sample * get_selected_sample() const;
  Pad * get_selected_pad() const;
};
