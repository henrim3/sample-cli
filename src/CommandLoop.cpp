#include "CommandLoop.h"

#include "App.h"
#include "IO.h"
#include "ModeResponse.h"

CommandLoop::CommandLoop( App & app, const Parser & parser )
    : _app( app ), _parser( parser ) {}

void CommandLoop::run() {
  _app.render();

  while ( true ) {
    Key key = IO::get_key();

    LoopBehavior loop_should = _app.handle_key( key );

    _app.render();

    switch ( loop_should ) {
      case LoopBehavior::KeepGoing:
        break;
      case LoopBehavior::Stop:
        IO::println( "Exiting..." );
        return;
      case LoopBehavior::StopOnError:
        IO::print_error( "Exiting on error!!" );
        return;
    }
  }
}
