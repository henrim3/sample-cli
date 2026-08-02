#include "DefaultKeyHandler.h"
#include "IO.h"
#include "ModeResponse.h"

DefaultKeyHandler::DefaultKeyHandler( const Parser & parser )
    : _parser( parser ) {}

ModeResponse DefaultKeyHandler::handle_key( Key key, AppContext & context ) {
  LineEditor & line_editor = context.state.line_editor;

  if ( !key.special_type.has_value() ) {
    line_editor.insert( key.s );
    return ModeResponse{};
  }

  switch ( key.special_type.value() ) {
    case SpecialKeyType::Unhandled:
      return ModeResponse{};

    case SpecialKeyType::ArrowLeft:
      line_editor.move_left();
      return ModeResponse{};

    case SpecialKeyType::ArrowRight:
      line_editor.move_right();
      return ModeResponse{};

    case SpecialKeyType::Backspace:
      context.state.line_editor.backspace();
      return ModeResponse{};

    case SpecialKeyType::Enter: {
      std::string line = std::string( line_editor.get_text() );
      line_editor.enter();
      IO::print_newline();
      MaybeAction maybe_action = _parser.parse_action( line );
      if ( !maybe_action.has_value() ) {
        IO::print_error( "Command not found" );
        return ModeResponse{};
      }
      return ModeResponse{ .parsed_action = maybe_action.value() };
    }

    case SpecialKeyType::ArrowDown:
    case SpecialKeyType::ArrowUp:
    case SpecialKeyType::Escape:
      return ModeResponse{};
  }
}
