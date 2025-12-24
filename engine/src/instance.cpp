#include "instance.h"
#include "ezrender.h"
#include "world.h"
//#include "mgui.h"
Instance* gInstance = nullptr;

void Instance::DoRendering() {
	gEzRender->RenderStart();
	if(gCurrentWorld) gCurrentWorld->RenderAllObjects();
	//if(MGUI::gCurrentCanvas) MGUI::gCurrentCanvas->UpdateElements();
	gEzRender->RenderEnd();

}

void Instance::Update() {
	if (mErrorHasOccured) {
		SDL_SetWindowTitle(mWindow, "An error has occured!");
		gEzRender->cSetDrawColor(255);
		SDL_RenderClear(gEzRender->mRenderer);
		gEzRender->cSetDrawColor();
		SDL_RenderDebugText(gEzRender->mRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "An error has occured, please restart the app.");
		gEzRender->RenderEnd();
		return;
	}

	static uint pCurrentFPS = 0;
	if (mStartingTicks == 0) mStartingTicks = SDL_GetTicks();
	pCurrentFPS++;

	//Run stuff here
	if (gCurrentWorld) {
		gCurrentWorld->UpdateAllObjects();
	}
	DoRendering();


	mCurrentTicks = SDL_GetTicks();
	if (mCurrentTicks - mStartingTicks >= 1000) {
		SDL_Log("Frames in one second: %d", pCurrentFPS);
		string_easy asStr = std::to_string(pCurrentFPS);
#ifdef COMOPT_C_CLIENT
		SDL_SetWindowTitle(mWindow, asStr.c_str());
#endif
		mFPS = pCurrentFPS;
		mStartingTicks = 0;
		pCurrentFPS = 0;
		

	}

	SDL_Delay(16);


}


Instance::Instance() {
	SDL_Log("Initilaizing instance...");

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		SDL_Log("Failed to initialize! (SDL_Init)");
		SDL_Quit();
	}
#if COMOPT_C_CLIENT
	mWindow = SDL_CreateWindow("Test", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!mWindow) {
		SDL_Log("Failed to initialize! (Window creation)");
		SDL_Quit();
	}

#endif

	gInstance = this;

	SDL_Log("Created instance (Success!)");
}