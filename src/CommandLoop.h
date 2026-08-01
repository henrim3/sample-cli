#pragma once

#include "App.h"
#include "IO.h"
#include "Parser.h"

enum class LoopResult {
  KEEP_GOING,
  STOP,
  ERROR,
};

class CommandLoop {
public:
  CommandLoop( App & app, const Parser & parser );

  void run();

private:
  App & _app;
  const Parser & _parser;

  LoopResult handle_special_key_pressed( SpecialKey key );
  LoopResult handle_action( const Action & action );
};
