#pragma once

#include "App.h"
#include "Parser.h"

enum class LoopResult {
  KeepGoing,
  Stop,
  Error,
};

class CommandLoop {
public:
  CommandLoop( App & app, const Parser & parser );

  void run();

private:
  App & _app;
  const Parser & _parser;

  LoopResult handle_special_key_pressed( SpecialKeyType key );
  LoopResult handle_action( const Action & action );
};
