#include "AppModeType.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "DefaultActionHandler.h"
#include "DefaultKeyHandler.h"
#include "HistoryManager.h"
#include "IO.h"
#include "LineEditor.h"
#include "Parser.h"
#include <optional>

int main() {
  IO::init();

  CommandRegistry command_registry{
    {
      .token = "deselect",
      .type = CommandType::Deselect,
      .is_global = true,
    },
    {
      .token = "exit",
      .type = CommandType::Quit,
    },
    {
      .token = "list",
      .type = CommandType::List,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "samples",
            .type = CommandType::ListSamples,
            .is_global = true,
          },
        },
    },
    {
      .token = "new",
      .type = CommandType::New,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "sample",
            .type = CommandType::NewSample,
            .arg_types =
              {
                CommandArgType::String,
              },
          },
        },
    },
    {
      .token = "play",
      .type = CommandType::Play,
    },
    {
      .token = "quit",
      .type = CommandType::Quit,
    },
    {
      .token = "select",
      .type = CommandType::Select,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "pad",
            .type = CommandType::SelectPad,
            .arg_types =
              {
                CommandArgType::SizeT,
              },
          },
          {
            .token = "sample",
            .type = CommandType::SelectSample,
            .arg_types =
              {
                CommandArgType::SizeT,
              },
          },
        },
    },
    {
      .token = "stop",
      .type = CommandType::Stop,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "all",
            .type = CommandType::StopAll,
            .is_global = true,
          },
        },
    },
  };

  Parser parser( command_registry );

  VoiceManager voice_manager;
  AudioEngine audio_engine( voice_manager );
  DeviceManager device_manager( audio_engine );
  FormatManager format_manager;
  SampleManager sample_manager( format_manager );
  PadManager pad_manager;

  AppServices services{ .voice_manager = voice_manager,
                        .sample_manager = sample_manager,
                        .pad_manager = pad_manager,
                        .audio_engine = audio_engine,
                        .device_manager = device_manager,
                        .format_manager = format_manager };

  LineEditor line_editor;
  HistoryManager history_manager;

  AppState state{ .mode = AppModeType::Project,
                  .selected_sample_id = std::nullopt,
                  .line_editor = line_editor,
                  .history_manager = history_manager };

  AppContext context = {
    .services = services,
    .state = state,
  };

  DefaultKeyHandler default_key_handler( parser );
  DefaultActionHandler default_action_handler;

  App app( context, default_key_handler, default_action_handler );
  CommandLoop command_loop( app, parser );

  command_loop.run();

  IO::destroy();

  return 0;
}
