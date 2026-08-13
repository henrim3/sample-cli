#include "AppContext.h"

Sample * AppContext::get_selected_sample() const {
  MaybeSizeT sample_id = state.selected_sample_id;
  if ( !sample_id.has_value() ) {
    return nullptr;
  }
  return services.sample_manager.get_sample_by_id( sample_id.value() );
}

Pad * AppContext::get_selected_pad() const {
  MaybeSizeT pad_id = state.selected_pad_id;
  if ( !pad_id.has_value() ) {
    return nullptr;
  }
  return services.pad_manager.get_pad_by_id( pad_id.value() );
}
