#pragma once

#include "Action.h"
#include "AppContext.h"

class IAppMode {
public:
  virtual ~IAppMode() = default;

  virtual ModeResponse handle_key( SpecialKey key, AppContext & context ) = 0;
  virtual ModeResponse handle_action( const Action & action,
                                      AppContext & context ) = 0;
};
