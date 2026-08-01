#include "DefaultKeyHandler.h"
#include "IO.h"
#include "ModeResponse.h"
#include <stdexcept>

DefaultKeyHandler::DefaultKeyHandler( const Parser & parser )
    : _parser( parser ) {}

ModeResponse DefaultKeyHandler::handle_key( SpecialKey key ) {
  switch ( key ) {
    case SpecialKey::COUNT:
      std::logic_error( "Got COUNT for key" );

    case SpecialKey::UNHANDLED:
      return ModeResponse{};

    case SpecialKey::BACKSPACE:
      IO::handle_backspace();
      return ModeResponse{};

    case SpecialKey::ENTER: {
      std::string line = std::string( IO::get_input_buffer() );
      IO::handle_enter();
      MaybeAction maybe_action = _parser.parse_action( line );
      if ( !maybe_action.has_value() ) {
        IO::print_error( "Command not found" );
        return ModeResponse{};
      }
      return ModeResponse{ .parsed_action = maybe_action.value() };
    }

    case SpecialKey::ESCAPE:
    case SpecialKey::ARROW_DOWN:
    case SpecialKey::ARROW_LEFT:
      IO::handle_left_arrow();
      return ModeResponse{};

    case SpecialKey::ARROW_RIGHT:
      IO::handle_right_arrow();
      return ModeResponse{};

    case SpecialKey::ARROW_UP:
      return ModeResponse{};
  }
}
