#pragma once

#include "common.h"

class Instance {
private:


	ulong mStartingTicks = 0;
	ulong mCurrentTicks = 0;

	bool mFPSLimit_Use = false;
	ushort mFPSLimit_Limit = 60;

	uint mFPS = 60;

	bool mErrorHasOccured = false;
public:
#if COMOPT_C_CLIENT
	SDL_Window* mWindow = nullptr;
	ushort mWidth = 0;
	ushort mHeight = 0;
#endif


	uint GetFPS() {
		return mFPS;
	}

	void UpdateWindowInformation() {
		int w = 0, h = 0;
		SDL_GetWindowSize(mWindow, &w, &h);
		mWidth = static_cast<ushort>(w);
		mHeight = static_cast<ushort>(h);

	}

	float GetDeltaTime() {
		//SDL_Log("Delta time: %f", 1 / static_cast<float>(mFPS));
		return 1 / static_cast<float>(mFPS);
	}

	void DoRendering();


	void Update();

	void ErrorHandle() {
		mErrorHasOccured = true;
	}

	Instance();
};

extern Instance* gInstance;

#define WINDOW_WIDTH = gInstance->mWidth
#define WINDOW_WIDTH_HALF = (gInstance->mWidth / 2)
#define WINDOW_HEIGHT = gInstance->mHeight
#define WINDOW_HEIGHT_HALF = (gInstance->mHeight / 2)