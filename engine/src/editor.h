#pragma once

#include "common.h"
#include "file.h"
#include "instance.h"
#include "world.h"
#include "object.h"
#include "input.h"
#include "ezrender.h"

struct ExportableMaxObject {
	Vector2 mStartPos;
	ushort mWidth = 0;
	ushort mHeight = 0;
	byte mID = 0;
	Property* mProperties = nullptr;
	SDL_Texture* mRenderImage = nullptr;
};

void uImportWorldSpace(const char* pFileName) {
	File f(o_read, pFileName);
	/* World space:
		0 - 4 = Header (MEWS) Max engine world space
		5 - 8 = BG Color
		9 = Gravity
		9 + (sizeof(olimit)) = Object count
		10 - end of file = Exportable max object data
	
	
	*/
}

#include "mgui.h"
#include <vector>
#include "input.h"




namespace Editor {

	namespace ObjectEditor {

	}
}