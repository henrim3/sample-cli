#pragma once

#include "ApplicationState.h"
#include "AudioEngine.h"
#include "DeviceManager.h"
#include "FormatManager.h"
#include "PadManager.h"
#include "Sample.h"
#include "SampleManager.h"
#include "VoiceManager.h"
#include <ostream>

class Application {
public:
  Application();

  SampleManager & get_sample_manager();
  ApplicationState & get_state();

  Sample * get_selected_sample();

  bool load_sample( std::string_view file_path );

  friend std::ostream & operator<<( std::ostream & os, Application a );

private:
  VoiceManager _voice_manager;
  AudioEngine _audio_engine{ _voice_manager };
  DeviceManager _device_manager{ _audio_engine };
  FormatManager _format_manager;
  SampleManager _sample_manager{ _format_manager };
  PadManager _pad_manager{};

  ApplicationState _state;
};
