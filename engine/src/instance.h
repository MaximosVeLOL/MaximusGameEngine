#pragma once

#include "common.h"

class Instance {
private:


	ulong mStartingTicks = 0;
	ulong mCurrentTicks = 0;

public:
#if COMOPT_C_CLIENT
	SDL_Window* mWindow = nullptr;
	ushort mWidth = 0;
	ushort mHeight = 0;
#endif

	void UpdateWindowInformation() {
#if COMOPT_C_CLIENT
		int w = 0, h = 0;
		SDL_GetWindowSize(mWindow, &w, &h);
		mWidth = static_cast<ushort>(w);
		mHeight = static_cast<ushort>(h);
#endif
	}

	void DoRendering();


	void Update();

	Instance();
};

extern Instance* gInstance;

#define WINDOW_WIDTH = gInstance->mWidth
#define WINDOW_WIDTH_HALF = (gInstance->mWidth / 2)
#define WINDOW_HEIGHT = gInstance->mHeight
#define WINDOW_HEIGHT_HALF = (gInstance->mHeight / 2)