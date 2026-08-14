#include "App.h"
#include "AppMode.h"
#include "AppState.h"
#include "DefaultEventHandler.h"
#include "EventHandler.h"
#include "EventHandlerRegistry.h"
#include "IO.h"
#include "SampleModeEventHandler.h"
#include <algorithm>

App::App( AppContext & context,
          const DefaultEventHandler & default_event_handler,
          const EventHandlerRegistry & event_handler_registry )
    : _context( std::move( context ) ),
      _default_event_handler( default_event_handler ),
      _event_handler_registry( event_handler_registry ) {}

LoopBehavior App::handle_key( Key key ) {
  const EventHandler * event_handler =
    _event_handler_registry.get_event_handler_for( _context.state.mode );

  ModeResponse res = event_handler->handle_key( key, _context );

  if ( res.switch_to_mode.has_value() ) {
    _context.state.mode = res.switch_to_mode.value();
  }

  if ( !res.was_consumed ) {
    ModeResponse res_from_default =
      _default_event_handler.handle_key( key, _context );

    if ( res_from_default.switch_to_mode.has_value() ) {
      _context.state.mode = res_from_default.switch_to_mode.value();
    }
    if ( res_from_default.parsed_action.has_value() ) {
      return handle_action( res_from_default.parsed_action.value() );
    }

    return std::max( res.loop_should, res_from_default.loop_should );
  }

  return res.loop_should;
}

LoopBehavior App::handle_action( const Action & action ) {
  ModeResponse res;
  if ( action.is_global ) {
    res = _event_handler_registry.get_global_event_handler()->handle_action(
      action, _context );
  } else {
    const EventHandler * event_handler =
      _event_handler_registry.get_event_handler_for( _context.state.mode );
    res = event_handler->handle_action( action, _context );
  }

  if ( res.switch_to_mode.has_value() ) {
    _context.state.mode = res.switch_to_mode.value();
  }

  if ( !res.was_consumed ) {
    ModeResponse default_res =
      _default_event_handler.handle_action( action, _context );

    if ( default_res.switch_to_mode.has_value() ) {
      _context.state.mode = default_res.switch_to_mode.value();
    }

    return std::max( { res.loop_should, default_res.loop_should } );
  }

  return res.loop_should;
}

const AppContext & App::get_context() const {
  return _context;
}

void App::render() const {
  IO::render( make_prompt(), _context.state.line_editor );
}

std::string App::make_prompt() const {
  std::string prompt = "sample-cli ";

  switch ( _context.state.mode ) {
    case AppMode::Main:
      prompt += "[main]";
      break;

    case AppMode::Project:
      prompt += "[project NAME]";
      break;

    case AppMode::Pad: {
      prompt += "[pad ";
      MaybeSizeT pad_id = _context.state.selected_pad_id;
      if ( pad_id.has_value() ) {
        prompt += std::to_string( pad_id.value() ) + "]";
      } else {
        prompt += "NONE]";
      }
      break;
    }

    case AppMode::Sample: {
      prompt += "[sample ";
      MaybeSizeT sample_id = _context.state.selected_sample_id;
      if ( sample_id.has_value() ) {
        prompt += std::to_string( sample_id.value() ) + "]";
      } else {
        prompt += "NONE]";
      }
      break;
    }
  }

  return prompt + " > ";
}
