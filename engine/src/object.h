#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "common.h"
#include "sprite.h"

struct Transform {
	Vector2 position;
	Vector2 velocity;
	ushort width = 50;
	ushort height = 50;
	float rotation = 0;
	byte mass = 1;
	char direction = 1;
	char speed = 1;


	operator Rect() {
		Rect r = {
			SDL_lround(position.x),
			SDL_lround(position.y),
			width,
			height,
		};

		return r;


	}

	Transform() {}
};

class MaxObject {
private:


public:
	
	uint mObjectID = 0;
	uint mWorldID = 0xFFFFFFFF;

	bool mIsActive = true;

	bool mUsePhysics = true;
	bool mStatic = true;
	bool mIsSolid = true;

	Transform mTransform;

	Sprite mSprite;
	bool mUseSprite = false;

	virtual void Step() {

	}

	void Update() {
		if (!mIsActive) return;
		if (mUseSprite) mSprite.Update();
		Step();
	}

	MaxObject();

};

extern MaxObject mAllObjects[];

#endif