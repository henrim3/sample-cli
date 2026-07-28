#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandRegistry.h"
#include "Input.h"
#include "Parser.h"

void _register_commands( CommandRegistry & command_registry ) {
  command_registry.register_command( {
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
  } );
}

int main() {
  CommandRegistry command_registry;

  _register_commands( command_registry );

  Application app;
  Input input;
  Parser parser( command_registry );
  CommandLoop command_loop( app, input, parser );

  command_loop.run();

  return 0;
}
