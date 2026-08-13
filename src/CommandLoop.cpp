#include "CommandLoop.h"

#include "App.h"
#include "IO.h"

CommandLoop::CommandLoop( App & app, const Parser & parser )
    : _app( app ), _parser( parser ) {}

void CommandLoop::run() {
  _app.render();

  while ( true ) {
    Key key = IO::get_key();

    LoopBehavior result = _app.handle_key( key );
    _app.render();

    switch ( result ) {
      case LoopBehavior::KeepGoing:
        break;
      case LoopBehavior::Stop:
        IO::println( "Exiting..." );
        return;
      case LoopBehavior::Error:
        IO::print_error( "Exiting on error!!" );
        return;
    }
  }
}
