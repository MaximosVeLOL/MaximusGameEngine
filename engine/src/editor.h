#pragma once


#include "file.h"

struct Property {
	string_static mName = "";
	void* value = nullptr;
};

struct ExportableMaxObject {
	Vector2 position;
	Property* properties = nullptr;
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



Property mProperties[]