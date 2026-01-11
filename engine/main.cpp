#define SDL_MAIN_USE_CALLBACKS 1

#include "src/common.h"
#include "src/instance.h"
#include "src/ezrender.h"
#include "src/world.h"
#include "src/object.h"
#include "src/input.h"
#include "src/mgui.h"
#include "src/assethandler.h"
#include "src/audiosystem.h"
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	gInstance = new Instance();
	gEzRender = new EzRender();
	gInput = new InputSystem();
	gAudio = new AudioSystem();
	gAssetHandler = new AssetHandler();
	gAssetHandler->LoadGroupByName("Test");
	
	gCurrentCanvas = new Canvas();
	eImage* test = new eImage();
	SDL_Surface* image = (SDL_Surface*)gAssetHandler->GetAssetData("test.png");
	SDL_assert(image);
	test->pImage = SDL_CreateTextureFromSurface(gEzRender->mRenderer, image);
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