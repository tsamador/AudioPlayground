#ifndef SYNESTHETIC_H
#define SYNESTHETIC_H

#include "sound_buffer.h"
#include "audio_engine.h"

struct application_state
{
	bool running;
};

void SynestheticMainLoop();

#endif