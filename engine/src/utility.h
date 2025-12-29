#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "common.h"
#include "instance.h"



void MessageBox(Vector2 pPosition, string_static pMessage) {
	
}

string_static uPrompt(string_static message) {
#if !COMOPT_C_CLIENT
	return;
#endif
	SDL_Window* popup = SDL_CreatePopupWindow(gInstance->mWindow, 0, 0, 400, 400, SDL_WINDOW_POPUP_MENU);
	popup
}


#endif