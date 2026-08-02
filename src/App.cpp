#include "App.h"
#include "AppMode.h"
#include "AppState.h"
#include "IO.h"
#include "ProjectMode.h"
#include "SampleMode.h"
#include <memory>

App::App( AppContext & context, const DefaultKeyHandler & default_key_handler,
          const DefaultActionHandler & default_action_handler )
    : _default_key_handler( default_key_handler ),
      _default_action_handler( default_action_handler ),
      _context( std::move( context ) ) {
  handle_mode_change( context.state.mode );
}

LoopBehavior App::handle_key( Key key ) {
  ModeResponse res = _mode->handle_key( key, _context );

  if ( res.switch_to_mode.has_value() ) {
    handle_mode_change( res.switch_to_mode.value() );
  }

  if ( !res.was_consumed ) {
    ModeResponse res_from_default =
      _default_key_handler.handle_key( key, _context );

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
      _default_action_handler.handle_action( action, _context );

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
      _default_action_handler.handle_action( action, _context );

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

void App::render() const {
  IO::render( make_prompt(), _context.state.line_editor );
}

void App::handle_mode_change( AppModeType mode ) {
  _context.state.mode = mode;

  switch ( mode ) {
    case AppModeType::Project:
      _mode = std::make_unique<ProjectMode>();
      break;
    case AppModeType::Sample:
      _mode = std::make_unique<SampleMode>();
      break;
  }
}

std::string App::make_prompt() const {
  std::string prompt = "sample-cli ";

  switch ( _context.state.mode ) {
    case AppModeType::Project:
      prompt += "[project]";
      break;
    case AppModeType::Sample: {
      prompt += "[sample ";

      MaybeSizeT sample_id = _context.state.selected_sample_id;
      if ( sample_id.has_value() ) {
        prompt += std::to_string( sample_id.value() ) + "]";
      } else {
        prompt += "NONE]";
      }
    }
  }

  return prompt + " > ";
}
