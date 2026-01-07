#pragma once


#include "file.h"

struct Property {
	string_static mName = "";
	void* value = nullptr;
};

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
enum Editor_Mode : byte  {
	EDITOR_MODE_DEFAULT = 0,
	EDITOR_MODE_PLACE, //When placing an object
	EDITOR_MODE_EDIT, //When editing an object
};

class Editor_cMain : public Canvas {
private:
	void Export() {

	}
public:

	Editor_Mode pMode = EDITOR_MODE_DEFAULT;
	byte pSelectedObjectID = 0;
	std::vector<ExportableMaxObject> pObjects;


	ExportableMaxObject* uGetObjectHovering() {
		Vector2 m = gInput->GetMousePos();
		for (ExportableMaxObject& o : pObjects) {
			if (m.x >= o.mStartX && m.x <= o.mStartX + o.mWidth && m.y >= o.mStartY && m.y <= o.mStartY + o.mHeight) {
				return &o;
			}
		}
	}


	void Update() {
		if (gInput->GetMouseDown(SDL_BUTTON_LEFT)) {
			switch (pMode) {
			case EDITOR_MODE_DEFAULT:
				break;

			case EDITOR_MODE_PLACE:
				ExportableMaxObject o;
				
				pObjects.push_back()
				break;

			case EDITOR_MODE_EDIT:

				break;
			}
		}
	}
}


//Property mProperties[]