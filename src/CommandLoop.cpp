#include "CommandLoop.h"

#include "App.h"
#include "IO.h"

CommandLoop::CommandLoop( App & app, const Parser & parser )
    : _app( app ), _parser( parser ) {}

void CommandLoop::run() {
  IO::print_prompt( _app.get_context() );

  while ( true ) {
    SpecialKey key = IO::get_special_key();

    LoopBehavior result = _app.handle_key( key );

    switch ( result ) {
      case LoopBehavior::Error:
        IO::print_error( "Exiting on error!!" );
        return;
      case LoopBehavior::Stop:
        IO::println( "Exiting..." );
        return;
      case LoopBehavior::KeepGoing:
        if ( key == SpecialKey::Enter ) {
          IO::print_prompt( _app.get_context() );
        }
        break;
      case LoopBehavior::KeepGoingNoPrompt:
        return;
    }
  }
}
