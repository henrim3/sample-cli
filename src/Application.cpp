#include "Application.h"
#include "Sample.h"
#include "SampleManager.h"

Application::Application() {}

SampleManager &Application::get_sample_manager() { return _sample_manager; }

ApplicationState &Application::get_state() { return _state; }

Sample *Application::get_selected_sample() {
  return _sample_manager.get_sample_by_id(_state.get_selected_sample_id());
}

std::ostream &operator<<(std::ostream &os, Application a) {
  return os << a._state;
}
