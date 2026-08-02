#pragma once

#include "AppContext.h"
#include "Key.h"
#include "ModeResponse.h"
#include "Parser.h"

class DefaultKeyHandler {
public:
  DefaultKeyHandler( const Parser & parser );

  ModeResponse handle_key( Key key, AppContext & context );

private:
  const Parser & _parser;
};
