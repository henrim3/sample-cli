#pragma once

#include "Action.h"
#include "AppContext.h"
#include "Key.h"
#include "ModeResponse.h"

class IAppMode {
public:
  virtual ~IAppMode() = default;

  virtual ModeResponse handle_key( Key key, AppContext & context ) = 0;
  virtual ModeResponse handle_action( const Action & action,
                                      AppContext & context ) = 0;
};
