#pragma once

#include "Action.h"
#include "AppContext.h"
#include "AppMode.h"
#include "DefaultActionHandler.h"
#include "DefaultKeyHandler.h"
#include "ModeResponse.h"
#include <ostream>

class App {
public:
  App( AppContext & context, const DefaultKeyHandler & default_key_handler,
       const DefaultActionHandler & default_action_handler );

  LoopBehavior handle_key( Key key );
  LoopBehavior handle_action( const Action & action );

  const AppContext & get_context() const;

  void render() const;

  friend std::ostream & operator<<( std::ostream & os, const App & a );

private:
  DefaultKeyHandler _default_key_handler;
  DefaultActionHandler _default_action_handler;

  AppContext _context;
  std::unique_ptr<IAppMode> _mode;

  void handle_mode_change( AppModeType mode_type );
  std::string make_prompt() const;
};
