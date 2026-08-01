#pragma once

#include "Action.h"
#include "AppModeType.h"
#include <optional>

enum class LoopBehavior {
  // in order of severity
  KeepGoing,
  KeepGoingNoPrompt,
  Stop,
  Error,
};

using MaybeAppModeType = std::optional<AppModeType>;

struct ModeResponse {
  LoopBehavior loop_should = LoopBehavior::KeepGoing;
  MaybeAction parsed_action = std::nullopt;
  MaybeAppModeType switch_to_mode = std::nullopt;
  bool was_consumed = false;
};
