#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "IO.h"

int main() {
  CommandRegistry command_registry{
    {
      .token = "exit",
      .type = CommandType::Quit,
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

  IO::init();

  Application app( command_registry );
  CommandLoop command_loop( app );

  command_loop.run();

  IO::destroy();

  return 0;
}
