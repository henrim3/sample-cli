#include "DefaultKeyHandler.h"
#include "IO.h"
#include "ModeResponse.h"
#include <stdexcept>

DefaultKeyHandler::DefaultKeyHandler( const Parser & parser )
    : _parser( parser ) {}

ModeResponse DefaultKeyHandler::handle_key( SpecialKey key ) {
  switch ( key ) {
    case SpecialKey::Count:
      std::logic_error( "Got Count for key" );

    case SpecialKey::Unhandled:
      return ModeResponse{};

    case SpecialKey::Backspace:
      IO::handle_backspace();
      return ModeResponse{};

    case SpecialKey::Enter: {
      std::string line = std::string( IO::get_input_buffer() );
      IO::handle_enter();
      MaybeAction maybe_action = _parser.parse_action( line );
      if ( !maybe_action.has_value() ) {
        IO::print_error( "Command not found" );
        return ModeResponse{};
      }
      return ModeResponse{ .parsed_action = maybe_action.value() };
    }

    case SpecialKey::Escape:
    case SpecialKey::ArrowDown:
    case SpecialKey::ArrowLeft:
      IO::handle_left_arrow();
      return ModeResponse{};

    case SpecialKey::ArrowRight:
      IO::handle_right_arrow();
      return ModeResponse{};

    case SpecialKey::ArrowUp:
      return ModeResponse{};
  }
}
