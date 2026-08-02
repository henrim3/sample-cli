#pragma once

#include "AppModeType.h"
#include "HistoryManager.h"
#include "LineEditor.h"
#include "TypeDefs.h"

struct AppState {
  AppModeType mode;
  MaybeSizeT selected_sample_id;
  LineEditor & line_editor;
  HistoryManager & history_manager;
};
