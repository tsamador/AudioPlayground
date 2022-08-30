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


struct audio_stream_attributes {
    IMMDeviceEnumerator *poinEnumerator = NULL;
    IMMDevice *poinDevice = NULL;
    IAudioClient *poinAudioClient = NULL;
    IAudioRenderClient *poinRenderClient = NULL;
    WAVEFORMATEX *poinWaveFormat = NULL;
};

HRESULT PlayAudioStream(sound_buffer* AudioStream, application_state* state);
audio_stream_attributes InitAudioEngine();
void ShutdownAudioEngine(audio_stream_attributes AudioClient); 

#endif