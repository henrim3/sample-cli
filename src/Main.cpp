#include <string>
#include <vector>

#include "Application.h"
#include "Command.h"
#include "CommandLoop.h"
#include "CommandsRepository.h"

std::vector<std::string> split_str( const std::string & s, char c ) {
  std::vector<std::string> v = {};
  std::string curr_str = "";
  for (std::size_t i = 0; i < s.size(); i++) {
    char curr_char = s[i];
    if (curr_char == c) {
      if (curr_str.size() != 0) {
        v.push_back( curr_str );
        curr_str = "";
      }
    } else {
      curr_str.push_back( curr_char );
    }
  }

  if (curr_str.size() != 0) {
    v.push_back( curr_str );
  }

  return v;
}

int main() {
  CommandsRepository commands;
  commands.register_command( {
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
  CommandLoop command_loop( app );

  command_loop.run();

  return 0;
}
