#include "DefaultEventHandler.h"
#include "IO.h"
#include "ModeResponse.h"

DefaultEventHandler::DefaultEventHandler( const Parser & parser )
    : _parser( parser ) {}

ModeResponse DefaultEventHandler::handle_key( Key key,
                                              AppContext & context ) const {
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
      MaybeStringView next_command = history_manager.newer_command();
      if ( next_command.has_value() ) {
        line_editor.set_text( next_command.value() );
      } else if ( context.state.is_history_active ) {
        history_manager.reset_cursor();
        context.state.is_history_active = false;
        line_editor.set_text( context.state.cached_input.value() );
        context.state.cached_input = std::nullopt;
      }

      line_editor.move_to_end();
      return ModeResponse{};
    }

    case SpecialKeyType::ArrowLeft:
      line_editor.move_left();
      return ModeResponse{};

    case SpecialKeyType::ArrowRight:
      line_editor.move_right();
      return ModeResponse{};

    case SpecialKeyType::ArrowUp: {
      MaybeStringView prev_command = history_manager.older_command();
      if ( prev_command.has_value() ) {
        // history is active
        if ( !context.state.is_history_active ) {
          context.state.is_history_active = true;
          context.state.cached_input = line_editor.get_text();
        }

        line_editor.set_text( prev_command.value() );
        line_editor.move_to_end();
      }

      return ModeResponse{};
    }

    case SpecialKeyType::Backspace:
      context.state.line_editor.backspace();
      return ModeResponse{};

    case SpecialKeyType::Enter: {
      // history inactive
      context.state.is_history_active = false;
      history_manager.reset_cursor();

      std::string line = std::string( line_editor.get_text() );

      context.state.history_manager.add_entry( line );

      line_editor.enter();
      IO::print_newline();

      // check if it's a global action first
      MaybeAction global_action = _parser.parse_global_action( line );
      if ( global_action.has_value() ) {
        return ModeResponse{ .parsed_action = global_action.value() };
      }

      // it's a mode action if not global
      MaybeAction mode_action =
        _parser.parse_mode_action( line, context.state.mode );
      if ( !mode_action.has_value() ) {
        IO::print_error( "Command not found" );
        return ModeResponse{};
      }
      return ModeResponse{ .parsed_action = mode_action.value() };
    }

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
