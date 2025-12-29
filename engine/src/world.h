#pragma once

class MaxObject;
#include "common.h"

typedef uint olimit;
constexpr olimit OBJECT_LIMIT = 10000;



class WorldSpace {
public:

	MaxObject* staticObjects;
	

	//When we need to load assets needed
	virtual void Precache() {

	}
};




class World {
	MaxObject* mObjects[OBJECT_LIMIT] = { nullptr };

	byte mGravity = 1;

public:
	olimit mObjectCount = 0;

	Vector2 mCameraPosition;

	Color mBGColor = {
		0,
		255,
		0,
		255
	};

	void AddObject(MaxObject* object) {
		if (mObjectCount >= OBJECT_LIMIT) {
			ERROR("We are above the object limit!");
		}
		mObjects[mObjectCount] = object;
		mObjectCount++;

	}

	MaxObject* uGetObjectInPoint(Vector2 pPoint);


	void UpdateAllObjects();

	void RenderAllObjects();

	World();

};

struct StaticMaxObject {
	const char* ID = "";
	Vector2 position;
};

extern World* gCurrentWorld;