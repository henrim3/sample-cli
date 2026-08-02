#include "PadMode.h"
#include "Command.h"
#include "IO.h"
#include "ModeResponse.h"

ModeResponse PadMode::handle_key( Key key, AppContext & context ) {
  if ( !key.special_type.has_value() ) {
    return ModeResponse{};
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( key.special_type.value() ) {
    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}

ModeResponse PadMode::handle_action( const Action & action,
                                     AppContext & context ) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.get_command_type() ) {
    case CommandType::NewPad: {
      Pad & pad = context.services.pad_manager.create_pad();
      IO::println( "Created pad ", pad.get_id() );
      return ModeResponse{};
    }

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
