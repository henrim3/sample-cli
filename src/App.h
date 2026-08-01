#pragma once

#include "Action.h"
#include "AppContext.h"
#include "AppMode.h"
#include "ModeResponse.h"
#include <ostream>

class App {
public:
  App( AppContext & context );

  LoopBehavior handle_key( SpecialKey key );
  LoopBehavior handle_action( const Action & action );

  const AppContext & get_context() const;

  friend std::ostream & operator<<( std::ostream & os, const App & a );

private:
  AppContext _context;
  std::unique_ptr<IAppMode> _mode;

  void handle_mode_change( std::optional<AppModeType> mode );
};
