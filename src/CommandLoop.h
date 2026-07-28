#pragma once

#include "Application.h"
#include "Input.h"
#include "Parser.h"

class CommandLoop {
public:
  CommandLoop( Application & app, const Input & input, const Parser & parser );

  void run();

private:
  Application & _app;
  const Input & _input;
  const Parser & _parser;
};
