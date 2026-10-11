#pragma once

#include <optional>
enum class CommandType {
  Deselect,

  ListCommands,

  // History
  ShowHistory,
  ShowFullHistory,

  Quit,

  // Samples
  ListSamples,
  NewSample,
  PlaySample,
  SelectSample,
  StopSample,

  // Pads
  AssignSample,
  NewPad,
  PlayPad,
  SelectPad,
  StopPad,

  StopAll,
};

using MaybeCommandType = std::optional<CommandType>;
