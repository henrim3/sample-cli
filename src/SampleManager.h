#pragma once

#include "FormatManager.h"
#include "Sample.h"
#include <unordered_map>

class SampleManager {
public:
  SampleManager( FormatManager & format_manager );

  Sample * load_sample( const std::string & file_path );
  Sample * get_sample_by_id( std::size_t id );

private:
  FormatManager & _format_manager;
  std::size_t _next_id = 0;
  std::unordered_map<std::size_t, Sample> _samples;
};
