#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include "sound_buffer.h"

HRESULT PlayAudioStream(sound_buffer* AudioStream);

#endif