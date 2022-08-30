

#include "synesthetic.h"
#include "gl_synes.h"
#include <thread>



void SynestheticMainLoop()
{
	application_state state;
	state.running = true;

	sound_buffer audioStream;
	
	std::thread audio_thread(PlayAudioStream, &audioStream, &state);
	

	while(state.running)
	{
		Render();

		ProcessInput(&state);
		
	}

	audio_thread.join();
}