#pragma once

#include <optional>
enum class CommandType {
  Deselect,

  Details,

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
