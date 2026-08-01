#include "DefaultActionHandler.h"
#include "AppContext.h"
#include "AppModeType.h"
#include "IO.h"
#include "ModeResponse.h"
#include <stdexcept>

ModeResponse DefaultActionHandler::handle_action( const Action & action,
                                                  AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::Count:
      throw std::logic_error( "Got Count as action" );

    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::SelectSample: {
      std::size_t id = action.get_arg<std::size_t>( 0 );
      if ( !context.sample_manager.has_sample( id ) ) {
        IO::print_error( "Sample ", id, " doesn't exist" );
        return ModeResponse{};
      }
      context.state.select_sample( id );
      return ModeResponse{ .switch_to_mode = AppModeType::Sample };
    }

    case CommandType::New:
    case CommandType::NewSample:
    case CommandType::Play:
    case CommandType::Select:
    case CommandType::SelectPad:
      return ModeResponse{};
  }
}
