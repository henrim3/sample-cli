#include "DefaultKeyHandler.h"
#include "HistoryManager.h"
#include "IO.h"
#include "ModeResponse.h"
#include "TypeDefs.h"

DefaultKeyHandler::DefaultKeyHandler( const Parser & parser )
    : _parser( parser ) {}

ModeResponse DefaultKeyHandler::handle_key( Key key, AppContext & context ) {
  LineEditor & line_editor = context.state.line_editor;
  HistoryManager & history_manager = context.state.history_manager;

  if ( !key.special_type.has_value() ) {
    // normal key
    line_editor.insert( key.s );
    context.state.is_history_active = false;
    history_manager.reset_cursor();
    return ModeResponse{};
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( key.special_type.value() ) {
    case SpecialKeyType::Unhandled:
      return ModeResponse{};

    case SpecialKeyType::ArrowDown: {
      MaybeStringView next_command = history_manager.next_command();
      if ( next_command.has_value() ) {
        line_editor.set_text( next_command.value() );
      } else {
        if ( context.state.is_history_active ) {
          history_manager.reset_cursor();
          context.state.is_history_active = false;
          line_editor.set_text( context.state.cached_input.value() );
          context.state.cached_input = std::nullopt;
        }
      }

      return ModeResponse{};
    }

    case SpecialKeyType::ArrowLeft:
      line_editor.move_left();
      return ModeResponse{};

    case SpecialKeyType::ArrowRight:
      line_editor.move_right();
      return ModeResponse{};

    case SpecialKeyType::ArrowUp: {
      MaybeStringView prev_command = history_manager.previous_command();
      if ( prev_command.has_value() ) {
        // history active
        if ( !context.state.is_history_active ) {
          context.state.is_history_active = true;
          context.state.cached_input = line_editor.get_text();
        }
        line_editor.set_text( prev_command.value() );
      }

      return ModeResponse{};
    }

    case SpecialKeyType::Backspace:
      context.state.line_editor.backspace();
      return ModeResponse{};

    case SpecialKeyType::Enter: {
      context.state.is_history_active = false;
      history_manager.reset_cursor();
      std::string line = std::string( line_editor.get_text() );
      context.state.history_manager.add_entry( line );
      line_editor.enter();
      IO::print_newline();
      MaybeAction maybe_action = _parser.parse_action( line );
      if ( !maybe_action.has_value() ) {
        IO::print_error( "Command not found" );
        return ModeResponse{};
      }
      return ModeResponse{ .parsed_action = maybe_action.value() };
    }

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
