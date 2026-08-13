#pragma once

#include "Action.h"
#include "AppContext.h"
#include "DefaultEventHandler.h"
#include "EventHandlerRegistry.h"
#include "ModeResponse.h"
#include <ostream>

class App {
public:
  App( AppContext & context, const DefaultEventHandler & default_event_handler,
       const EventHandlerRegistry & event_handler_registry );

  LoopBehavior handle_key( Key key );
  LoopBehavior handle_action( const Action & action );

  const AppContext & get_context() const;

  void render() const;

  friend std::ostream & operator<<( std::ostream & os, const App & a );

private:
  AppContext _context;
  const DefaultEventHandler & _default_event_handler;
  const EventHandlerRegistry & _event_handler_registry;

  std::string make_prompt() const;
};
