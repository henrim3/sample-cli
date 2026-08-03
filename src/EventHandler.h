#pragma once

#include "Action.h"
#include "AppContext.h"
#include "Key.h"
#include "ModeResponse.h"

class EventHandler {
public:
  virtual ~EventHandler() = default;

  virtual ModeResponse handle_key( Key key, AppContext & context ) const {
    (void)key;
    (void)context;
    return ModeResponse{};
  }
  virtual ModeResponse handle_action( const Action & action,
                                      AppContext & context ) const {
    (void)action;
    (void)context;
    return ModeResponse{};
  }
};
