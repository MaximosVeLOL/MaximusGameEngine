#ifndef __MFP_H__
#define __MFP_H__


#include "string.h"
#include "stdio.h"

/*	Xx Max's Fast Parser. xX (Start: 9:4? PM)
	
	JSON uses javascript
	But this uses properties.
	[name] : [value]

*/

typedef char* string_editable;
typedef const char* string_static;
typedef unsigned char byte;



enum VariableType : byte {
	VARIABLE_TYPE_NUMBER = 0,
	VARIABLE_TYPE_BOOLEAN = 1,
	VARIABLE_TYPE_STRING = 2,
};

struct Property {
	char mName[128] = {'\0'};
	void* mData = nullptr;
	VariableType mType = VARIABLE_TYPE_NUMBER;

	Property(string_static pName, string_static pData) {
		mType = VARIABLE_TYPE_STRING;
		for (byte i = 0; i < strlen(pName); i++) {
			mName[i] = pName[i];
		}
		mData = pData;
	}
	Property(string_static pName, int value) {
		mType = VARIABLE_TYPE_NUMBER;
		int* val = new int(value);
		mData = val;
	}
	Property(string_static pName, bool value, bool dontUseThisParameterPlease = false) {
		mType = VARIABLE_TYPE_BOOLEAN;
		bool* val = malloc
		mData = val;

	}
};



void Test() {
	Property property("test", 0);
}

class MFP_Section {
private:
	Property* mData = NULL;
public:
	public void AddProperty(Property pIn) {
		if (mData == NULL) {
			mData = 
		}
	}

	public void Export(string_static pFileName) {
		FILE* file = NULL;
		fopen_s(&file, pFileName, "w");
		fprintf_s(file, "%s:%d")
	}
};

#endif