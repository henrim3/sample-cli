#include "EventHandlerRegistry.h"

void EventHandlerRegistry::register_global_event_handler(
  const EventHandler & handler ) {
  _global_event_handler = &handler;
}

void EventHandlerRegistry::register_event_handler_for(
  AppMode mode, const EventHandler & event_handler ) {
  _mode_event_handlers[mode] = &event_handler;
}

const EventHandler * EventHandlerRegistry::get_global_event_handler() const {
  return _global_event_handler;
}

const EventHandler *
EventHandlerRegistry::get_event_handler_for( AppMode mode ) const {
  return _mode_event_handlers.at( mode );
}
