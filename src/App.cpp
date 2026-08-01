#include "App.h"
#include "AppMode.h"
#include "AppState.h"
#include "ProjectMode.h"
#include "SampleMode.h"
#include <memory>

App::App( AppContext & context ) : _context( std::move( context ) ) {
  handle_mode_change( context.state.get_mode_type() );
}

LoopBehavior App::handle_key( SpecialKey key ) {
  ModeResponse res = _mode->handle_key( key, _context );

  if ( res.switch_to_mode.has_value() ) {
    handle_mode_change( res.switch_to_mode.value() );
  }

  if ( !res.was_consumed ) {
    ModeResponse res_from_default =
      _context.default_key_handler.handle_key( key );

    if ( res_from_default.switch_to_mode.has_value() ) {
      handle_mode_change( res_from_default.switch_to_mode.value() );
    }

    if ( res_from_default.parsed_action.has_value() ) {
      return handle_action( res_from_default.parsed_action.value() );
    }
  }

  return res.loop_should;
}

LoopBehavior App::handle_action( const Action & action ) {
  // if it's a global command just use default
  if ( action.is_global() ) {
    ModeResponse default_res =
      _context.default_action_handler.handle_action( action, _context );

    if ( default_res.switch_to_mode.has_value() ) {
      handle_mode_change( default_res.switch_to_mode.value() );
    }

    return default_res.loop_should;
  }

    ModeResponse res = _mode->handle_action( action, _context );

  if ( res.switch_to_mode.has_value() ) {
    handle_mode_change( res.switch_to_mode.value() );
  }

  if ( !res.was_consumed ) {
    ModeResponse default_res =
      _context.default_action_handler.handle_action( action, _context );

    if ( default_res.switch_to_mode.has_value() ) {
      handle_mode_change( default_res.switch_to_mode.value() );
    }

    return default_res.loop_should;
  }

  return res.loop_should;
}

const AppContext & App::get_context() const {
  return _context;
}

std::ostream & operator<<( std::ostream & os, const App & a ) {
  return os << a._context.state;
}

void App::handle_mode_change( MaybeAppModeType maybe_mode_type ) {
  if ( maybe_mode_type.has_value() ) {
    AppModeType mode_type = maybe_mode_type.value();

    _context.state.set_mode_type( mode_type );

    switch ( mode_type ) {
      case AppModeType::Project:
        _mode = std::make_unique<ProjectMode>();
        break;
      case AppModeType::Sample:
        _mode = std::make_unique<SampleMode>();
        break;
    }
  }
}
