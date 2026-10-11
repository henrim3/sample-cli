#pragma once

#include "CommandRegistry.h"
#include "EventHandler.h"

class GlobalEventHandler : public EventHandler {
public:
  GlobalEventHandler( const CommandRegistry & commandRegistry );

  ModeResponse handle_action( const Action & action,
                              AppContext & context ) const override;

private:
  const CommandRegistry & _commandRegistry;

  void output_all_commands( const Commands & commands,
                            std::size_t indent ) const;
};
