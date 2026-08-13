#pragma once

#include "AppMode.h"
#include "EventHandler.h"
#include <unordered_map>

class EventHandlerRegistry {
public:
  void register_global_event_handler( const EventHandler & handler );
  void register_event_handler_for( AppMode mode,
                                   const EventHandler & event_handler );

  const EventHandler * get_global_event_handler() const;
  const EventHandler * get_event_handler_for( AppMode mode ) const;

private:
  const EventHandler * _global_event_handler;
  std::unordered_map<AppMode, const EventHandler *> _mode_event_handlers;
};
