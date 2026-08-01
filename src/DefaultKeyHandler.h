#pragma once

#include "ModeResponse.h"
#include "Parser.h"
#include "SpecialKey.h"

struct AppContext;

class DefaultKeyHandler {
public:
  DefaultKeyHandler( const Parser & parser );

  ModeResponse handle_key( SpecialKey key );

private:
  const Parser & _parser;
};
