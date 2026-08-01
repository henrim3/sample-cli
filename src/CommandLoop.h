#pragma once

#include "Application.h"

class CommandLoop {
public:
  CommandLoop( Application & app );

  void run();

private:
  Application & _app;
};
