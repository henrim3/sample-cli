#include "DefaultActionHandler.h"
#include "AppContext.h"
#include "AppModeType.h"
#include "IO.h"
#include "ModeResponse.h"
#include <stdexcept>

ModeResponse DefaultActionHandler::handle_action( const Action & action,
                                                  AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::COUNT:
      throw std::logic_error( "Got COUNT as action" );

    case CommandType::QUIT:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::SELECT_SAMPLE: {
      std::size_t id = action.get_arg<std::size_t>( 0 );
      if ( !context.sample_manager.has_sample( id ) ) {
        IO::print_error( "Sample ", id, " doesn't exist" );
        return ModeResponse{};
      }
      context.state.select_sample( id );
      return ModeResponse{ .switch_to_mode = AppModeType::Sample };
    }

    case CommandType::NEW:
    case CommandType::NEW_SAMPLE:
    case CommandType::PLAY:
    case CommandType::SELECT:
    case CommandType::SELECT_PAD:
      return ModeResponse{};
  }
}
