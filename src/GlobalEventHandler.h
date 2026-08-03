#pragma once

#include "EventHandler.h"

class GlobalEventHandler : public EventHandler {
  ModeResponse handle_action( const Action & action,
                              AppContext & context ) const override;
};
