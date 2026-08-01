#pragma once

#include "Action.h"
#include "ModeResponse.h"

struct AppContext;

class DefaultActionHandler {
public:
  ModeResponse handle_action( const Action & action,
                              AppContext & context );
};
