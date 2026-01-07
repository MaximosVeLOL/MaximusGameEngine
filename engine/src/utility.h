#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "common.h"
#include "instance.h"



void MessageBox(Vector2 pPosition, string_static pMessage) {
	
}

/* Throw an error which will do things depending on the status.
* pStatus -
*	0 = Bad, core issue, will prompt an error message and destroy everything.
*	1 = Ok, we failed, but we will restart/try again.
*	2 = Fine, we can't do this, so we will do something else. 
* pMessage - A regular message, if you want to use formatted strings, use a max string.

*/
void Error(byte pStatus, string_static pMessage) {
	SDL_IOStream* errorOut = SDL_IOFromFile("./ERROR.txt", "a");
	if (errorOut) {
		SDL_IOprintf(errorOut, "(time: %d, status: %d): %s", SDL_GetTicks(), pStatus, pMessage);
		SDL_CloseIO(errorOut);
	}
	switch (pStatus) {
		case 0:
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A (CORE) error has occured!", pMessage, NULL);
			break;
		case 1:

			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A (Not OK) error has occured!", pMessage, NULL);
			break;
		case 2:
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A (Fine) error has occured!", pMessage, NULL);
			break;
		default:
	}
}

string_static uPrompt(string_static message) {
#if COMOPT_C_CLIENT


	SDL_Window* popup = SDL_CreatePopupWindow(gInstance->mWindow, 0, 0, 400, 400, SDL_WINDOW_POPUP_MENU);
	
#endif
}


#endif