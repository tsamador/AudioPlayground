#ifndef SOUND_BUFFER_H
#define SOUND_BUFFER_H
	
#include <windows.h>
#include <math.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>


#define PI  3.141592653589793238462643383279502884L

struct sound_buffer {

	WAVEFORMATEX format;
	float* buffer;
	unsigned int bufferSize;
	unsigned int currentBuffLocation;
	unsigned int numChannels;
	unsigned int SampleRate;

	/*The LoadData function writes a specified number of audio frames
	  (first parameter) to a specified buffer location (second parameter)
	*/
	HRESULT LoadData(unsigned int bufferFrameCount, void* data, DWORD *flags);
	void Init(unsigned int SampleRate, double freq, int NumChannels, unsigned int bitsPerSample );
	HRESULT SetFormat(WAVEFORMATEX *newFormat);
};

#endif
