#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "Input.h"
#include "Parser.h"

int main() {
  CommandRegistry command_registry{
    {
      .token = "exit",
      .type = CommandType::QUIT,
    },
    {
      .token = "quit",
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
          },
        },
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
        },
    },
  };

  Application app;
  Input input;
  Parser parser( command_registry );
  CommandLoop command_loop( app, input, parser );

  command_loop.run();

  return 0;
}
