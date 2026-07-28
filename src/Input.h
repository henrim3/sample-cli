#pragma once

#include <string>
#include <termios.h>

class Input {
public:
  Input();
  ~Input();

  char get_ch() const;

  std::string get_line() const;

private:
  termios oldt;
};
