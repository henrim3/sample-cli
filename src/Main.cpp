#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "IO.h"
#include "Parser.h"

int main() {
  CommandRegistry command_registry{
    {
      .token = "exit",
      .type = CommandType::QUIT,
    },
    {
      .token = "new",
      .type = CommandType::NEW,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "sample",
            .type = CommandType::NEW_SAMPLE,
            .arg_types =
              {
                CommandArgType::STR,
              },
          },
        },
    },
    {
      .token = "play",
      .type = CommandType::PLAY,
    },
    {
      .token = "quit",
      .type = CommandType::QUIT,
    },
    {
      .token = "select",
      .type = CommandType::SELECT,
      .is_phony = true,
      .subcommands =
        {
          {
            .token = "pad",
            .type = CommandType::SELECT_PAD,
            .arg_types =
              {
                CommandArgType::SIZE_T,
              },
          },
          {
            .token = "sample",
            .type = CommandType::SELECT_SAMPLE,
            .arg_types =
              {
                CommandArgType::SIZE_T,
              },
          },
        },
    },
  };

  IO::init();

  Application app;
  Parser parser( command_registry );
  CommandLoop command_loop( app, parser );

  command_loop.run();

  IO::destroy();

  return 0;
}
