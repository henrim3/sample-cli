#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ApplicationState.h"

typedef std::vector<std::string> Tokens;

Tokens split_str(std::string s, char c) {
  std::vector<std::string> v = {};
  std::string curr_str = "";
  for (size_t i = 0; i < s.size(); i++) {
    char curr_char = s[i];
    if (curr_char == c) {
      if (curr_str.size() != 0) {
        v.push_back(curr_str);
        curr_str = "";
      }
    } else {
      curr_str.push_back(curr_char);
    }
  }

  if (curr_str.size() != 0) {
    v.push_back(curr_str);
  }

  return v;
}

void print_str_vector(Tokens &v) {
  std::cout << "String Vector of size " << v.size() << std::endl;
  for (std::string s : v) {
    std::cout << s << std::endl;
  }
}

bool expect_num_tokens(Tokens &tokens, size_t n) {
  if (tokens.size() != n) {
    std::cout << "Command expects " << n << "args" << std::endl;
    return false;
  }
  return true;
}

bool try_parse_int(std::string s, int &i) {
  try {
    i = std::stoi(s);
    return true;
  } catch (const std::invalid_argument &) {
    return false;
  }
}

void handle_select_command(Tokens &tokens, ApplicationState &state) {
  if (!expect_num_tokens(tokens, 2)) {
    return;
  }

  int i;
  if (!try_parse_int(tokens[1], i)) {
    std::cout << "Sample id must be an int" << std::endl;
    return;
  }

  state.set_selected_sample_id(i);

  std::cout << "Selected sample " << i << std::endl;
}

// returns true if keep running, false if exit
bool handle_input(std::string input, ApplicationState &state) {
  Tokens tokens = split_str(input, ' ');

  if (tokens.size() == 0) {
    std::cout << "No input" << std::endl;
    return true;
  }

  if (tokens[0] == "exit") {
    return false;
  } else if (tokens[0] == "select") {
    handle_select_command(tokens, state);
  }

  std::cout << std::endl << state << std::endl << std::endl;

  return true;
}

void main_loop() {
  ApplicationState state;

  while (true) {
    std::cout << "sample-cli > ";
    std::string input;
    std::getline(std::cin, input);
    if (!handle_input(input, state)) {
      return;
    }
  }
}

int main() {
  main_loop();
  return 0;
}
