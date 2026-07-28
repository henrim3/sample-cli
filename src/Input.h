#pragma once

#include <termios.h>

class Input {
public:
  Input();
  ~Input();

  char get_ch() const;

private:
  termios oldt;
};
