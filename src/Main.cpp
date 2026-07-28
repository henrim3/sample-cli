#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "Input.h"
#include "Output.h"
#include "Parser.h"

int main() {
  CommandRegistry command_registry;
  command_registry.register_command( {
    .type = CommandType::SELECT,
    .token = "select",
    .subcommands =
      {
        {
          .type = CommandType::SELECT_PAD,
          .token = "pad",
          .arg_types =
            {
              CommandArgType::SIZE_T,
            },
        },
      },
  } );

  Application app;
  Input input;
  Parser parser( command_registry );
  Output::init();
  CommandLoop command_loop( app, input, parser );

  command_loop.run();

  return 0;
}
