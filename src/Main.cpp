#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "DefaultActionHandler.h"
#include "DefaultKeyHandler.h"
#include "IO.h"
#include "Parser.h"

int main() {
  IO::init();

  CommandRegistry command_registry{
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
  };

  Parser parser( command_registry );

  VoiceManager voice_manager;
  AudioEngine audio_engine( voice_manager );
  DeviceManager device_manager( audio_engine );
  FormatManager format_manager;
  SampleManager sample_manager( format_manager );
  PadManager pad_manager;
  DefaultKeyHandler default_key_handler( parser );
  DefaultActionHandler default_action_handler;
  AppState state( AppModeType::Project );

  AppContext context = {
    .voice_manager = voice_manager,
    .audio_engine = audio_engine,
    .device_manager = device_manager,
    .format_manager = format_manager,
    .sample_manager = sample_manager,
    .pad_manager = pad_manager,
    .default_key_handler = default_key_handler,
    .default_action_handler = default_action_handler,
    .state = state,
  };

  App app( context );
  CommandLoop command_loop( app, parser );

  command_loop.run();

  IO::destroy();

  return 0;
}
