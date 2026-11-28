#include "AppMode.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "DotDisplay.h"
#include "EventHandlerRegistry.h"
#include "GlobalEventHandler.h"
#include "HistoryManager.h"
#include "IO.h"
#include "LineEditor.h"
#include "MediaQuery.h"
#include "PadModeEventHandler.h"
#include "Parser.h"
#include "PlaybackEngine.h"
#include "ProjectModeEventHandler.h"
#include "SampleModeEventHandler.h"
#include "SequenceModeEventHandler.h"

int main() {
  IO::init();

  MaybeScreenDimensions term_dimensions = MediaQuery::get_terminal_dimensions();

  if ( !term_dimensions.has_value() ) {
    IO::print_error( "Couldn't get terminal dimensions" );
    return 1;
  }

  DotDisplay d;
  d.set_output_buffer_dimensions( term_dimensions.value().width,
                                  term_dimensions.value().height );
  d.set_graphics_buffer_dimensions( 2880, 1800 );

  d.draw_square( 10, 10, 200, 200 );

  d.output();

  return 0;
}

/*
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
                .token = "pad",
                .type = CommandType::NewPad,
              },
              {
                .token = "sample",
                .type = CommandType::NewSample,
                .params =
                  {
                    {
                      .name = "filename",
                      .type = CommandParamType::String,
                    },
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
                .params =
                  {
                    {
                      .name = "pad id",
                      .type = CommandParamType::SizeT,
                    },
                  },
              },
              {
                .token = "sample",
                .type = CommandType::SelectSample,
                .params =
                  {
                    {
                      .name = "sample id",
                      .type = CommandParamType::SizeT,
                    },
                  },
              },
              {
                .token = "sequence",
                .type = CommandType::SelectSequence,
              },
            },
        },
      },
    },
    {
      AppMode::Pad,
      {
        {
          .token = "assign",
          .subcommands =
            {
              {
                .token = "sample",
                .type = CommandType::AssignSample,
                .params =
                  {
                    {
                      .name = "sample id",
                      .type = CommandParamType::SizeT,
                    },
                  },
              },
            },
        },
        {
          .token = "play",
          .type = CommandType::PlayPad,
        },
        {
          .token = "stop",
          .type = CommandType::StopPad,
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
    {
      AppMode::Sequence,
      {
        {
          .token = "new",
          .subcommands =
            {
              {
                .token = "note",
                .type = CommandType::NewNote,
                // pad id, start, duration
                .params =
                  {
                    {
                      .name = "pad id",
                      .type = CommandParamType::SizeT,
                    },
                    {
                      .name = "start",
                      .type = CommandParamType::NonNegativeDouble,
                    },
                    {
                      .name = "duration",
                      .type = CommandParamType::NonNegativeDouble,
                    },
                    {
                      .name = "one shot",
                      .type = CommandParamType::Bool,
                    },
                  },
              },
            },
        },
        {
          .token = "play",
          .type = CommandType::PlaySequence,
        },
        {
          .token = "set",
          .subcommands =
            {
              {
                .token = "length",
                .type = CommandType::SetSequenceLength,
              },
            },
        },
        {
          .token = "stop",
          .type = CommandType::StopSequence,
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
        .token = "debug",
        .subcommands =
          {
            {
              .token = "dimensions",
              .type = CommandType::DebugTerminalDimensions,
            },
          },
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
        .token = "list",
        .subcommands =
          {
            {
              .token = "commands",
              .type = CommandType::ListCommands,
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

  PadManager pad_manager;
  VoiceManager voice_manager;
  PlaybackEngine playback_engine( pad_manager, voice_manager );
  AudioEngine audio_engine( playback_engine );
  DeviceManager device_manager( audio_engine );
  FormatManager format_manager;
  SampleManager sample_manager( format_manager );
  DotDisplay dot_display;

  AppServices services{ .voice_manager = voice_manager,
                        .sample_manager = sample_manager,
                        .pad_manager = pad_manager,
                        .audio_engine = audio_engine,
                        .device_manager = device_manager,
                        .format_manager = format_manager,
                        .playback_engine = playback_engine,
                        .dot_display = dot_display };

  LineEditor line_editor;
  HistoryManager history_manager;

  AppState state{ .mode = AppMode::Project,
                  .line_editor = line_editor,
                  .history_manager = history_manager };

  AppContext context = {
    .services = services,
    .state = state,
  };

  // init event handlers
  DefaultEventHandler default_event_handler( parser );
  GlobalEventHandler global_event_handler( command_registry );
  ProjectModeEventHandler project_mode_event_handler;
  PadModeEventHandler pad_mode_event_handler;
  SampleModeEventHandler sample_mode_event_handler;
  SequenceModeEventHandler sequence_mode_event_handler;

  EventHandlerRegistry event_handler_registry;

  // register event handlers
  event_handler_registry.register_global_event_handler( global_event_handler
  ); event_handler_registry.register_event_handler_for(
    AppMode::Project, project_mode_event_handler );
  event_handler_registry.register_event_handler_for( AppMode::Pad,
                                                     pad_mode_event_handler
                                                     );
  event_handler_registry.register_event_handler_for(
    AppMode::Sample, sample_mode_event_handler );
  event_handler_registry.register_event_handler_for(
    AppMode::Sequence, sequence_mode_event_handler );

  App app( context, default_event_handler, event_handler_registry );
  CommandLoop command_loop( app, parser );

  command_loop.run();

  IO::destroy();

  return 0;
}
*/
