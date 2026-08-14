#pragma once

#include "App.h"
#include "Parser.h"

class CommandLoop {
public:
  CommandLoop( App & app, const Parser & parser );

  void run();

private:
  App & _app;
  const Parser & _parser;
};
