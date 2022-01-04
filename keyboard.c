#include "keyboard.h"



void updateInput(void) {
        // message processing loop

 	SDL_PumpEvents();
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//quit (for debug)
	if(state[SDL_SCANCODE_END] || state[SDL_SCANCODE_ESCAPE]){
		exit(0);
	}
}

