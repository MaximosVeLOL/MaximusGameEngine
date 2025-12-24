#include "common.h"

class enum VariableType {
	Number = 0,
	Boolean = 1,
	String = 2,
};


struct ConsoleVariable {
	VariableType mType;
	void* mValue = nullptr;

	ConsoleVariable(byte pStringSize, string_static pStartingValue = "") {
		mType = VariableType::String;
		mValue = SDL_malloc(pStringSize);
		memset(mValue, pStartingValue, sizeof(pStartingValue));
	}

	ConsoleVariable(VariableType startingType) {
	}

	~ConsoleVariable() {
		SDL_free(mValue);
	}
};