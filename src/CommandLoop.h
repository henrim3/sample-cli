#pragma once

#include "Application.h"
#include "Input.h"
#include "Parser.h"

enum class LoopResult {
  KEEP_GOING,
  STOP,
  ERROR,
};

class CommandLoop {
public:
  CommandLoop( Application & app, const Input & input, const Parser & parser );

  void run();

private:
  Application & _app;
  const Input & _input;
  const Parser & _parser;

  LoopResult handle_action( const Action & action );
};
