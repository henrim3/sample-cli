#pragma once

#include "Action.h"
#include "ApplicationState.h"
#include "AudioEngine.h"
#include "CommandRegistry.h"
#include "DeviceManager.h"
#include "FormatManager.h"
#include "IO.h"
#include "PadManager.h"
#include "Parser.h"
#include "Sample.h"
#include "SampleManager.h"
#include "VoiceManager.h"
#include <ostream>

enum class AppResponse {
  KeepGoing,
  Stop,
  Error,
};

class Application {
public:
  Application( const CommandRegistry & command_registry );

  AppResponse handle_special_key_pressed( SpecialKey key );
  AppResponse handle_action( const Action & action );

  SampleManager & get_sample_manager();
  ApplicationState & get_state();

  Sample * get_selected_sample();

  // SAMPLE
  AppResponse select_sample( std::size_t id );
  AppResponse load_sample( std::string_view file_path );
  AppResponse play_current_sample();

  // CORE
  AppResponse play();

  friend std::ostream & operator<<( std::ostream & os, Application a );

private:
  Parser _parser;
  VoiceManager _voice_manager;
  AudioEngine _audio_engine{ _voice_manager };
  DeviceManager _device_manager{ _audio_engine };
  FormatManager _format_manager;
  SampleManager _sample_manager{ _format_manager };
  PadManager _pad_manager{};

  ApplicationState _state;
};
