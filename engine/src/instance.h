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
#endif

	uint GetFPS() {
		return mFPS;
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