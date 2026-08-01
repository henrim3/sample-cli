#pragma once

#include "AppState.h"
#include "AudioEngine.h"
#include "DefaultActionHandler.h"
#include "DefaultKeyHandler.h"
#include "DeviceManager.h"
#include "FormatManager.h"
#include "PadManager.h"
#include "SampleManager.h"
#include "VoiceManager.h"

struct AppContext {
  VoiceManager & voice_manager;
  AudioEngine & audio_engine;
  DeviceManager & device_manager;
  FormatManager & format_manager;
  SampleManager & sample_manager;
  PadManager & pad_manager;
  DefaultKeyHandler & default_key_handler;
  DefaultActionHandler & default_action_handler;
  AppState & state;
};
