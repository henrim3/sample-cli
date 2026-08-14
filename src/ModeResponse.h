#pragma once

#include "Action.h"
#include "AppMode.h"
#include <optional>

enum class LoopBehavior {
  // in order of severity
  KeepGoing,
  Stop,
  StopOnError,
};

struct ModeResponse {
  LoopBehavior loop_should = LoopBehavior::KeepGoing;
  MaybeAction parsed_action = std::nullopt;
  MaybeAppModeType switch_to_mode = std::nullopt;
  bool was_consumed = false;
};
