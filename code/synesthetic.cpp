 

#include "synesthetic.h"
#include "gl_synes.h"




void SynestheticMainLoop()
{
	application_state state;
	state.running = true;

	while(state.running)
	{
		Render();

		ProcessInput(&state);
		
	}
}