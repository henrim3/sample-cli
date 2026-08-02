#pragma once

#include "AppModeType.h"
#include "HistoryManager.h"
#include "LineEditor.h"
#include "TypeDefs.h"
#include <optional>

struct AppState {
  AppModeType mode;
  MaybeSizeT selected_sample_id;
  bool is_history_active = false;
  MaybeString cached_input = std::nullopt;
  LineEditor & line_editor;
  HistoryManager & history_manager;
};
