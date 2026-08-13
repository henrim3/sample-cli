#pragma once

#include "AppMode.h"
#include "HistoryManager.h"
#include "LineEditor.h"
#include "TypeDefs.h"

struct AppState {
  AppMode mode;
  MaybeSizeT selected_sample_id = std::nullopt;
  MaybeSizeT selected_pad_id = std::nullopt;
  bool is_history_active = false;
  MaybeString cached_input = std::nullopt;
  LineEditor & line_editor;
  HistoryManager & history_manager;
};
