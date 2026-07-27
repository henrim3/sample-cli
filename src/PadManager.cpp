#include "PadManager.h"
#include "Pad.h"

Pad &PadManager::create_pad() {
  std::size_t id = _next_id++;
  _pads.insert({id, Pad(id)});
  return _pads.at(id);
}

Pad *PadManager::get_pad_by_id(std::size_t id) {
  try {
    return &_pads.at(id);
  } catch (std::out_of_range &) {
    return nullptr;
  }
}
