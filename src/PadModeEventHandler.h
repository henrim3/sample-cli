#pragma once

#include "AppContext.h"
#include "EventHandler.h"
#include "ModeResponse.h"

class PadModeEventHandler : public EventHandler {
  ModeResponse handle_action( const Action & action,
                              AppContext & context ) const override;
};
