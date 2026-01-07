#pragma once

class MaxObject;
#include "common.h"
#include <vector> //For uGetObjectsInView

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

	float mGravity = 0.25;

	ushort mTickRate = 0;
	ushort mTicks = 0;

public:
	olimit mObjectCount = 0;

	Vector2 mCameraPosition;

	Color mBGColor = {
		0,
		255,
		0,
		255
	};

	void AddObject(MaxObject* object);

	MaxObject* uGetObjectInPoint(Vector2 pPoint);
	std::vector<MaxObject *> uGetObjectsInView();

	void UpdateAllObjects();

	void RenderAllObjects();

	World();

};

struct StaticMaxObject {
	const char* ID = "";
	Vector2 position;
};

extern World* gCurrentWorld;