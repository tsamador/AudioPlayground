
#include "sound_buffer.h"

HRESULT sound_buffer::LoadData(unsigned int bufferFrameCount, void* data, DWORD *flags)
{

	float* bufferToWrite = (float*)data;
	unsigned int currentLoc = 0;
	for(int i = 0; i < bufferFrameCount; i++)
	{	
		bufferToWrite[i] = this->buffer[this->currentBuffLocation++];
		if(currentBuffLocation >= this->bufferSize)
		{
			currentBuffLocation = 0;
			//flags = (DWORD*)AUDCLNT_BUFFERFLAGS_SILENT;
		}
	}
	return NULL;
}

HRESULT sound_buffer::SetFormat(WAVEFORMATEX *newFormat)
{
	//TODO Change our format to our own struct
	format.wFormatTag = newFormat->wFormatTag;
	format.nChannels = newFormat->nChannels;
	format.nSamplesPerSec = newFormat->nSamplesPerSec;
	format.nAvgBytesPerSec = newFormat->nAvgBytesPerSec;
	format.nBlockAlign = newFormat->nBlockAlign;
	format.wBitsPerSample = newFormat->wBitsPerSample;
	format.cbSize = newFormat->cbSize;

	Init(format.nSamplesPerSec, 440, format.nChannels, format.wBitsPerSample);
	return 0;
}

//Allocate a buffer of "bytes" and fill it with one second of a sinewave
void sound_buffer::Init(unsigned int SampleRate, double freq, int NumChannels, unsigned int bitsPerSample )
{
	this->bufferSize = (SampleRate * NumChannels);
	this->buffer = (float*) malloc((bitsPerSample/8) * SampleRate * NumChannels);
	
	//TODO(Tanner) Add support for multiple channels (This assumes one channel for now)
	int bufferPointer = 0;
	for(int i = 0; i < SampleRate; i++)
	{
		float sample = sin(2 * PI * freq * (i / (float)SampleRate));
		this->buffer[bufferPointer++] = sample;
		this->buffer[bufferPointer++] = sample;
	}

	this->currentBuffLocation = 0;
	
}

