#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include "application_state.h"
#include "synesthetic.h"
#include "sound_buffer.h"
#include "combaseapi.h"
#include "timeapi.h"




HRESULT PlayAudioStream(sound_buffer* AudioStream, application_state* state);

#endif