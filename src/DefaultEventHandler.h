#pragma once

#include "EventHandler.h"
#include "Parser.h"

class DefaultEventHandler : public EventHandler {
public:
  DefaultEventHandler( const Parser & parser );

  ModeResponse handle_key( Key key, AppContext & context ) const override;
  ModeResponse handle_action( const Action & action,
                              AppContext & context ) const override;

private:
  const Parser & _parser;
};
