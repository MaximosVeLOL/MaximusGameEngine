#include "instance.h"
#include "ezrender.h"
#include "world.h"
#include "mgui.h"
#include "input.h"
Instance* gInstance = nullptr;




void Instance::DoRendering() {
	gEzRender->RenderStart();
	if(gCurrentWorld) gCurrentWorld->RenderAllObjects();
	if(gCurrentCanvas) gCurrentCanvas->RenderElements();
	

}

void Instance::Update() {

	//Run stuff here
	if (gCurrentWorld) {
		gCurrentWorld->UpdateAllObjects();
	}
	if (gCurrentCanvas) {
		gCurrentCanvas->UpdateElements();
	}
	if (gInput) {
		gInput->UpdateAll();
	}

	DoRendering();

	SDL_Delay(16);


}


Instance::Instance() {
	SDL_Log("Initilaizing instance...");

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		SDL_Log("Failed to initialize! (SDL_Init)");
		SDL_Quit();
	}
#if COMOPT_C_CLIENT
	mWidth = 960;
	mHeight = 540;
	mWindow = SDL_CreateWindow("Test", mWidth, mHeight, SDL_WINDOW_RESIZABLE);
	if (!mWindow) {
		SDL_Log("Failed to initialize! (Window creation)");
		SDL_Quit();
	}

#endif

	gInstance = this;

	SDL_Log("Created instance (Success!)");
}