#include "AppMode.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "EventHandlerRegistry.h"
#include "GlobalEventHandler.h"
#include "HistoryManager.h"
#include "IO.h"
#include "LineEditor.h"
#include "Parser.h"
#include "ProjectModeEventHandler.h"
#include "SampleModeEventHandler.h"
#include <optional>

int main() {
  IO::init();

  // register commands for each mode
  CommandRegistry command_registry{
    {
      AppMode::Main,
      {},
    },
    {
      AppMode::Project,
      {
        {
          .token = "list",
          .subcommands =
            {
              {
                .token = "samples",
                .type = CommandType::ListSamples,
              },
            },
        },
        {
          .token = "new",
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
          .token = "select",
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
      },
    },
    {
      AppMode::Sample,
      {
        {
          .token = "play",
          .type = CommandType::PlaySample,
        },
        {
          .token = "stop",
          .type = CommandType::StopSample,
        },
      },
    },
  };

  // register global commands (work in any mode)
  command_registry.register_global_commands( {
    {
      {
        .token = "deselect",
        .type = CommandType::Deselect,
      },
      {
        .token = "exit",
        .type = CommandType::Quit,
      },
      {
        .token = "history",
        .type = CommandType::ShowHistory,
        .subcommands =
          {
            {
              .token = "full",
              .type = CommandType::ShowFullHistory,
            },
          },
      },
      {
        .token = "quit",
        .type = CommandType::Quit,
      },
      {
        .token = "stop",
        .subcommands = { {
          .token = "all",
          .type = CommandType::StopAll,
        } },
      },
    },
  } );

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

  AppState state{ .mode = AppMode::Project,
                  .selected_sample_id = std::nullopt,
                  .line_editor = line_editor,
                  .history_manager = history_manager };

  AppContext context = {
    .services = services,
    .state = state,
  };

  // init event handlers
  DefaultEventHandler default_event_handler( parser );
  GlobalEventHandler global_event_handler;
  ProjectModeEventHandler project_mode_event_handler;
  SampleModeEventHandler sample_mode_event_handler;

  EventHandlerRegistry event_handler_registry;

  // register event handlers
  event_handler_registry.register_global_event_handler( global_event_handler );
  event_handler_registry.register_event_handler_for(
    AppMode::Project, project_mode_event_handler );
  event_handler_registry.register_event_handler_for(
    AppMode::Sample, sample_mode_event_handler );

  App app( context, default_event_handler, event_handler_registry );
  CommandLoop command_loop( app, parser );

  command_loop.run();

  IO::destroy();

  return 0;
}
