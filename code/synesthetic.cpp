 

#include "synesthetic.h"
#include "gl_synes.h"




void SynestheticMainLoop()
{
	application_state state;
	state.running = true;

	sound_buffer audioStream;
	PlayAudioStream(&audioStream);

	while(state.running)
	{
		Render();

		ProcessInput(&state);
		
	}
}