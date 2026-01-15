#define SDL_MAIN_USE_CALLBACKS 1


#include <SDL3/SDL_main.h>
#include "src/instance.h"
#include "src/ezrender.h"
#include "src/mgui.h"
#include "src/common.h"
#include "src/world.h"
#include "src/audiosystem.h"
#include "src/input.h"
#include "src/assethandler.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	gInstance = new Instance();
#if COMOPT_R_USE
	gEzRender = new EzRender();
#endif

#if COMOPT_I_USE
	gInput = new InputSystem();
#endif
#if COMOPT_S_USE
	gAudio = new AudioSystem();
#endif

#ifdef COMOPT_A_USE
	gAssetHandler = new AssetHandler();
#endif
	gAssetHandler->LoadGroupByName("Test");
	gCurrentCanvas = new Canvas();
	eImage* test = new eImage();
	SDL_Surface* image = (SDL_Surface*)gAssetHandler->GetAssetData("test.png");
	SDL_assert(image);
	test->pImage = image;
	//test->pImage = SDL_CreateTextureFromSurface(gEzRender->mRenderer, image);
	SDL_assert(test->pImage);
	test->mRect = Rect(200, 200, 400, 400);
	gCurrentCanvas->AddElement(test);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		break;

		case SDL_EVENT_WINDOW_RESIZED:
			gInstance->UpdateWindowInformation();
		break;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	gInstance->Update();
	gEzRender->RenderEnd();
	//SDL_Delay(16);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

}