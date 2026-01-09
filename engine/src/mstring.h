#include "common.h"


//A string that has resizing and stuff like std::string, but also includes easier things like adding to format, ect.
struct MaxString {
	string_editable mValue = nullptr;
	ushort mStringLength = 0;

	void Resize(ushort newLength) {

	}


	MaxString(string_static pStartValue) {
		mValue = new char[sizeof(pStartValue)];
		strcpy(mValue, pStartValue);
		SDL_strlcpy(mValue, pStartValue, sizeof(mValue));

	}
};