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

class eButton_Test : public eButton {
public:
	void OnClicked() override {
		gCurrentCanvas->GetElementByID("Text")->uToggleVisibility();
	}
};

class oPlayer : public MaxObject {
public:
	void Step() override {
		if (gInput->GetInputDown(INPUT_CODE_DOWN) ) {
			mTransform.velocity.y = -20;
		}
		char moveX = (gInput->GetInputHeld(INPUT_CODE_RIGHT) - gInput->GetInputHeld(INPUT_CODE_LEFT));
		if (moveX != 0) {
			static byte delay = 4;
			mTransform.direction = moveX;
			delay++;

			if (delay >= 4) {
				mTransform.speed = (mTransform.speed < 6 ? mTransform.speed + 1 : 6);
				delay = 0;
			}

			if (mSprite.mCellIndex != 0) mSprite.SetCellIndex(0); //Run animation
		}
		else if (mSprite.mCellIndex != 1) mSprite.SetCellIndex(1); //Idle animation
		if (moveX != mTransform.direction) {
			
			mTransform.speed = 0;
		}
		
		
		mTransform.velocity.x = mTransform.speed * moveX;
		gCurrentWorld->mCameraPosition = Vector2(
			mTransform.position.x - (960 / 2) - 25,
			mTransform.position.y - (540 / 2) - 50
		);
	}

	oPlayer() {
		mTransform.width = 64;
		mTransform.height = 64;
		mTransform.position.x = 200;
		//mUsePhysics = false;
		mUseSprite = true;
		//mIsActive = false;
	}
};

class oObjectCreator : public MaxObject {
public:

	bool pPlay = false;

	oPlayer* pPlayer = nullptr;

	Vector2 pPreviousMousePos = Vector2(-1, -1);

	void Step() override {
		if (gInput->GetMouseHeld(SDL_BUTTON_RIGHT)) {

			MaxObject* touching = gCurrentWorld->uGetObjectInPoint(gInput->GetMousePos());
			if (touching) {
				if (pPreviousMousePos.x >= 0) {
					touching->mTransform.position.x += gInput->GetMousePos().x - pPreviousMousePos.x;
					touching->mTransform.position.y += gInput->GetMousePos().y - pPreviousMousePos.y;
					touching->mTransform.velocity = Vector2(0, 0);
				}
			}
			pPreviousMousePos = gInput->GetMousePos();
		}
		else pPreviousMousePos = Vector2(-1, -1);
		if (gInput->GetMouseDown(SDL_BUTTON_LEFT)) {
			MaxObject* n = new MaxObject();
			n->mTransform.position = gInput->GetMousePos();
			n->mTransform.width = 100;
			n->mTransform.height = 100;
			gCurrentWorld->AddObject(n);
		}
		static byte delay = 0;
		delay++;
		if (delay >= 10) {
			MaxObject* o = new MaxObject();
			o->mTransform.position.x = SDL_rand(10000);
			o->mTransform.position.y = 0;
			delay = 0;
			SDL_Log("New object!");
			gCurrentWorld->AddObject(o);
		}
		
		gEzRender->cRenderText(20, 20, "Object count: %d", gCurrentWorld->mObjectCount);

		//gCurrentWorld->mCameraPosition.x += (gInput->GetInputHeld(INPUT_CODE_RIGHT) - gInput->GetInputHeld(INPUT_CODE_LEFT)) * 5.0f;
		//gCurrentWorld->mCameraPosition.y += (gInput->GetInputHeld(INPUT_CODE_DOWN) - gInput->GetInputHeld(INPUT_CODE_UP)) * 5.0f;
	}
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	gInstance = new Instance();
	gEzRender = new EzRender();
	gInput = new InputSystem();
	gAudio = new AudioSystem();

	gAssetHandler = new AssetHandler();
	
	gCurrentWorld = new World();
	gCurrentCanvas = new Canvas();

	gAssetHandler->LoadGroup(ASSET_GROUP_TEST);

	oPlayer* p = new oPlayer();

	oObjectCreator* oCreator = new oObjectCreator();
	oCreator->pPlayer = p;
	p->mSprite = *(Sprite *)gAssetHandler->GetAssetData("plr.MESF");
	
	gCurrentWorld->AddObject(oCreator);
	MaxObject* o = new MaxObject();
	o->mTransform.position = Vector2(0, 600);
	o->mTransform.width = 10000;
	o->mTransform.height = 100;
	o->mUsePhysics = false;
	gCurrentWorld->AddObject(o);
	gCurrentWorld->AddObject(p);
	
	Sound* s = new Sound(nullptr);
	s->mAudioData = (MIX_Audio*)gAssetHandler->GetAssetData("bg.wav");
	if (!s->mAudioData) {
		SDL_Log("Failed to load sound!");
	}
	else gAudio->PlaySound(*s, 0);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	if (event->type == SDL_EVENT_WINDOW_RESIZED) gInstance->UpdateWindowInformation();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	gInstance->Update();
	//SDL_Delay(16);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

}