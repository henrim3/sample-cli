#include "Parser.h"

Parser::Parser( const CommandRegistry & command_registry )
    : _command_registry( command_registry ) {}

    /*
std::vector<Action> parse_actions( std::string_view s ) const {
  // lineize
  // tokenize each line
  // return actions
}

std::vector<std::string> Parser::tokenize( std::string_view s ) const {}

std::vector<std::string>
Parser::split_str( std::string_view s,
                   const std::vector<char> & delimiters ) const {
  // std::vector<std::string> v = {};
  // std::string curr_str = "";
  // for (std::size_t i = 0; i < s.size(); i++) {
  //   char curr_char = s[i];
  //   if (curr_char == c) {
  //     if (curr_str.size() != 0) {
  //       v.push_back( curr_str );
  //       curr_str = "";
  //     }
  //   } else {
  //     curr_str.push_back( curr_char );
  //   }
  // }
  //
  // if (curr_str.size() != 0) {
  //   v.push_back( curr_str );
  // }
  //
  // return v;
}
*/
