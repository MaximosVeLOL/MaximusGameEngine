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
	gEzRender->RenderEnd();

}

void Instance::Update() {
	if (mErrorHasOccured) {
		SDL_SetWindowTitle(mWindow, "An error has occured!");
		gEzRender->cSetDrawColor(255);
		SDL_RenderClear(gEzRender->mRenderer);
		gEzRender->cSetDrawColor();
		SDL_RenderDebugText(gEzRender->mRenderer, mWidth / 2, mHeight / 2, "An error has occured, please restart the app.");
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
	if (gCurrentCanvas) {
		gCurrentCanvas->UpdateElements();
	}
	if (gInput) {
		gInput->UpdateAll();
	}

	DoRendering();


	mCurrentTicks = SDL_GetTicks();
	if (mCurrentTicks - mStartingTicks >= 1000) {
		//SDL_Log("Frames in one second: %d", pCurrentFPS);
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