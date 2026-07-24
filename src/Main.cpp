#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ApplicationState.h"
#include "AudioDeps.h"

typedef std::vector<std::string> Tokens;

Tokens split_str(const std::string &s, char c) {
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

void print_str_vector(const Tokens &v) {
  std::cout << "String Vector of size " << v.size() << std::endl;
  for (std::string s : v) {
    std::cout << s << std::endl;
  }
}

void print_prompt(ApplicationState &state) {
  std::cout << "sample-cli";
  if (state.get_mode() == ApplicationMode::Sample) {
    std::cout << " [sample " << state.get_selected_sample_id() << "]";
  }
  std::cout << "> ";
}

void print_error(std::string_view s) {
  std::cout << "ERROR: " << s << std::endl;
}

bool expect_num_tokens(const Tokens &tokens, size_t n) {
  if (tokens.size() != n) {
    return false;
  }
  return true;
}

bool try_parse_int(const std::string &s, int &i) {
  try {
    i = std::stoi(s);
    return true;
  } catch (const std::invalid_argument &) {
    return false;
  }
}

void handle_select_sample(const Tokens &tokens, ApplicationState &state) {
  if (tokens.size() > 3) {
    std::cout << "select sample expects up to 1 argument" << std::endl;
    return;
  }

  if (tokens.size() == 2) {
    if (!state.select_last_sample()) {
      std::cout << "No previous sample to select" << std::endl;
      return;
    }

    std::cout << "Selected sample " << state.get_selected_sample_id()
              << std::endl;
    return;
  }

  int i;
  if (!try_parse_int(tokens[2], i)) {
    std::cout << "Sample id must be an int" << std::endl;
    return;
  }

  if (!state.select_sample((size_t)i)) {
    std::cout << "Couldn't select sample " << i << std::endl;
    return;
  }

  std::cout << "Selected sample " << i << std::endl;
}

void handle_select_command(const Tokens &tokens, ApplicationState &state) {
  if (tokens.size() < 2) {
    std::cout << "select expects at least 2 arguments" << std::endl;
    return;
  }

  if (tokens[1] == "sample") {
    handle_select_sample(tokens, state);
  } else {
    std::cout << "Unsupported select command" << std::endl;
  }
}

void handle_deselect(const Tokens &tokens, ApplicationState &state) {
  if (!expect_num_tokens(tokens, 1)) {
    std::cout << "deselect doesn't take any arguments" << std::endl;
  }

  state.deselect_sample();
}

void handle_new_sample(const Tokens &tokens, ApplicationState &state) {
  if (!expect_num_tokens(tokens, 2)) {
    std::cout << "select sample expects up to 1 argument" << std::endl;
    return;
  }

  Sample *sample = state.create_sample();

  std::cout << "Created sample " << sample->get_id() << std::endl;
}

void handle_new_command(const Tokens &tokens, ApplicationState &state) {
  if (tokens.size() < 2) {
    std::cout << "new expects at least 2 arguments" << std::endl;
    return;
  }

  if (tokens[1] == "sample") {
    handle_new_sample(tokens, state);
  } else {
    std::cout << "Unsupported new command" << std::endl;
  }
}

void handle_load_sample(const Tokens &tokens, ApplicationState &state) {
  if (!expect_num_tokens(tokens, 2)) {
    std::cout << "File name must be provided" << std::endl;
    return;
  }

  std::string_view file_name = tokens[1];

  std::cout << "Loading file " << file_name << std::endl;
  Sample *sample = state.get_current_sample();

  if (sample == nullptr) {
    print_error("Couldn't get sample");
    return;
  }

  if (!sample->load_file(tokens[1])) {
    print_error("Couldn't load file");
    return;
  }
}

void handle_sample_command(const Tokens &tokens, ApplicationState &state) {
  if (tokens[0] == "load") {
    handle_load_sample(tokens, state);
  }
}

void handle_inner_command(const Tokens &tokens, ApplicationState &state) {
  switch (state.get_mode()) {
  case ApplicationMode::Sample:
    handle_sample_command(tokens, state);
    break;
  case ApplicationMode::Main:
  default:
    std::cout << "Unsupported command" << std::endl;
    break;
  }
}

// returns true if keep running, false if exit
bool handle_input(std::string input, ApplicationState &state) {
  Tokens tokens = split_str(input, ' ');

  if (tokens.size() == 0) {
    std::cout << "No input" << std::endl;
    return true;
  }

  // commands for any mode
  if (tokens[0] == "exit") {
    std::cout << "Exiting..." << std::endl;
    return false;
  } else if (tokens[0] == "select") {
    handle_select_command(tokens, state);
  } else if (tokens[0] == "deselect") {
    handle_deselect(tokens, state);
  } else if (tokens[0] == "new") {
    handle_new_command(tokens, state);
  } else {
    handle_inner_command(tokens, state);
  }

  std::cout << std::endl << state << std::endl << std::endl;

  return true;
}

void main_loop() {
  AudioDeps audioDeps;
  ApplicationState state(audioDeps);

  while (true) {
    print_prompt(state);
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
