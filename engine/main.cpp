#define SDL_MAIN_USE_CALLBACKS 1

#include "src/common.h"
#include "src/instance.h"
#include "src/ezrender.h"
#include "src/world.h"
#include "src/object.h"
#include "src/input.h"
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	gInstance = new Instance();
	gEzRender = new EzRender();
	gInput = new InputSystem();
	/*
	gCurrentWorld = new World();
	gCurrentWorld->AddObject(new TestObject());
	MaxObject* o = new MaxObject();
	o->mTransform.position = Vector2(300, 100);
	o->mTransform.height = 500;
	o->mUsePhysics = false;
	gCurrentWorld->AddObject(o);
	o = new MaxObject();
	o->mTransform.position = Vector2(0, 400);
	o->mTransform.width = 960;
	o->mUsePhysics = false;

	gCurrentWorld->AddObject(o);
	*/
	/*
	gCurrentCanvas = new Canvas();
	eBar *b = new eBar();
	b->mRect = Rect(200, 200, 100, 50);
	gCurrentCanvas->AddElement(b);
	*/
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	gInstance->Update();
	gInput->UpdateAll();
	//SDL_Delay(16);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

}