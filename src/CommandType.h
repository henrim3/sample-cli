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

  // Sequence
  NewNote,
  PlaySequence,
  SelectSequence,
  SetSequenceLength,
  StopSequence,

  StopAll,
};

using MaybeCommandType = std::optional<CommandType>;
