#include "CommandLoop.h"

#include "Application.h"
#include "IO.h"

CommandLoop::CommandLoop( Application & app )
    : _app( app ) {}

void CommandLoop::run() {
  IO::print_prompt( _app.get_state() );

  while ( true ) {
    SpecialKey key = IO::get_special_key();

    AppResponse result = _app.handle_special_key_pressed( key );

    switch ( result ) {
      case AppResponse::Error:
        IO::print_error( "Exiting on error!!" );
        return;
      case AppResponse::Stop:
        IO::println( "Exiting..." );
        return;
      case AppResponse::KeepGoing:; // no-op
    }
  }
}
