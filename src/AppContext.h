#pragma once

#include "AppServices.h"
#include "AppState.h"

struct AppContext {
  AppServices & services;
  AppState & state;
};
