#pragma once

#include "EventHandler.h"

class ProjectModeEventHandler : public EventHandler {
public:
  ModeResponse handle_key( Key key, AppContext & context ) const override;
  ModeResponse handle_action( const Action & action,
                              AppContext & context ) const override;
};
