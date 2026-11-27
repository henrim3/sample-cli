#include <cstddef>

struct SequenceEvent {
  std::size_t noteId;
  double beat;
  std::size_t padId;

  enum Type { Start, Stop };

  Type type;
};
