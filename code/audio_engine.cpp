
#include "audio_engine.h"

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

HRESULT PlayAudioStream(sound_buffer* AudioStream, application_state* state)
{
	HRESULT winResult;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    UINT32 numFramesPadding;
    BYTE *sharedBuffer;
    DWORD flags = 0;

    //Increases granularity of the windows scheduler
    timeBeginPeriod(1);
    
    REFERENCE_TIME hnsActualDuration = REFTIMES_PER_SEC;

    audio_stream_attributes AudioClient = InitAudioEngine();

    // Tell the audio source which format to use.
    winResult = AudioStream->SetFormat(AudioClient.poinWaveFormat);
    EXIT_ON_ERROR(winResult)

    // Get the actual size of the allocated buffer.
    winResult = AudioClient.poinAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(winResult)

    winResult = AudioClient.poinAudioClient->GetService(
                         IID_IAudioRenderClient,
                         (void**)&AudioClient.poinRenderClient);
    EXIT_ON_ERROR(winResult)

    // Grab the entire buffer for the initial fill operation.
    winResult = AudioClient.poinRenderClient->GetBuffer(bufferFrameCount, &sharedBuffer);
    EXIT_ON_ERROR(winResult)

    // Load the initial data into the shared buffer.
    winResult = AudioStream->LoadData(bufferFrameCount, sharedBuffer, &flags);
    EXIT_ON_ERROR(winResult)

    winResult = AudioClient.poinRenderClient->ReleaseBuffer(bufferFrameCount, flags);
    EXIT_ON_ERROR(winResult)

    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (double)REFTIMES_PER_SEC *
                        bufferFrameCount / AudioClient.poinWaveFormat->nSamplesPerSec;

    winResult = AudioClient.poinAudioClient->Start();  // Start playing.
    EXIT_ON_ERROR(winResult)

    // Each loop fills about half of the shared buffer.
    while(state->running)
    {
        // Sleep for half the buffer duration.
        Sleep((DWORD)(hnsActualDuration/REFTIMES_PER_MILLISEC/2));

        // See how much buffer space is available.
        winResult = AudioClient.poinAudioClient->GetCurrentPadding(&numFramesPadding);
        EXIT_ON_ERROR(winResult)

        numFramesAvailable = bufferFrameCount - numFramesPadding;

        // Grab all the available space in the shared buffer.
        winResult = AudioClient.poinRenderClient->GetBuffer(numFramesAvailable, &sharedBuffer);
        EXIT_ON_ERROR(winResult)

        // Get next 1/2-second of data from the audio source.
        winResult = AudioStream->LoadData(numFramesAvailable, sharedBuffer, &flags);
        EXIT_ON_ERROR(winResult)

        winResult = AudioClient.poinRenderClient->ReleaseBuffer(numFramesAvailable, flags);
        EXIT_ON_ERROR(winResult)
    }

    // Wait for last data in buffer to play before stopping.
    Sleep((DWORD)(hnsActualDuration/REFTIMES_PER_MILLISEC/2));

    winResult = AudioClient.poinAudioClient->Stop();  // Stop playing.
    EXIT_ON_ERROR(winResult)

Exit:
    ShutdownAudioEngine(AudioClient);
    return winResult;
}


audio_stream_attributes InitAudioEngine() 
{
    HRESULT winResult;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    audio_stream_attributes AudioStream;

    // Initialize the COM model on this thread
    CoInitializeEx(0, COINIT_MULTITHREADED); 

    /* 
        Initialize a COM object, that is a device enumerator
        to see all possible multimedia devices
    */
    winResult = CoCreateInstance(
           CLSID_MMDeviceEnumerator, NULL,
           CLSCTX_ALL, IID_IMMDeviceEnumerator,
           (void**)&AudioStream.poinEnumerator); 
    EXIT_ON_ERROR(winResult)

    /* 
        Based on the enumerator, get the default audio endpoint,
        tie it to PDevice
    */
    winResult = AudioStream.poinEnumerator->GetDefaultAudioEndpoint(
                        eRender, eConsole, &AudioStream.poinDevice);
    EXIT_ON_ERROR(winResult)

    /*
        Activate the audio device in the program, tie it to 
        pAudioClient  
    */
    winResult = AudioStream.poinDevice->Activate(
                    IID_IAudioClient, CLSCTX_ALL,
                    NULL, (void**)&AudioStream.poinAudioClient);
    EXIT_ON_ERROR(winResult)

    /*
        Get the Format of the mix format of the 
    */
    winResult = AudioStream.poinAudioClient->GetMixFormat(&AudioStream.poinWaveFormat);
    EXIT_ON_ERROR(winResult)

    /*
        Initialize the AudioClient 
    */
    winResult = AudioStream.poinAudioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         0,
                         hnsRequestedDuration,
                         0,
                         AudioStream.poinWaveFormat,
                         NULL);
    EXIT_ON_ERROR(winResult)

Exit:
    if(winResult != S_OK)
    {
        ShutdownAudioEngine(AudioStream);
    }
    return AudioStream;
}

void ShutdownAudioEngine(audio_stream_attributes AudioClient) 
{

    CoTaskMemFree(AudioClient.poinWaveFormat);
    SAFE_RELEASE(AudioClient.poinEnumerator)
    SAFE_RELEASE(AudioClient.poinDevice)
    SAFE_RELEASE(AudioClient.poinAudioClient)
    SAFE_RELEASE(AudioClient.poinRenderClient)

}