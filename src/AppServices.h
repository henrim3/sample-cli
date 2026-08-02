#pragma once

#include "AudioEngine.h"
#include "DeviceManager.h"
#include "FormatManager.h"
#include "PadManager.h"
#include "SampleManager.h"
#include "VoiceManager.h"

struct AppServices {
  VoiceManager & voice_manager;
  SampleManager & sample_manager;
  PadManager & pad_manager;
  AudioEngine & audio_engine;
  DeviceManager & device_manager;
  FormatManager & format_manager;
};
