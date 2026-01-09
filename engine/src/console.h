#include "common.h"

enum VariableType : byte {
	VARIABLE_TYPE_BOOL = 0,
	VARIABLE_TYPE_INT = 1,
	VARIABLE_TYPE_STRING = 2,
};

struct ConsoleVariable {
	void* value = nullptr;
	VariableType type = VARIABLE_TYPE_STRING;


	ConsoleVariable(uint pStartingValue) {
		value = SDL_malloc(4);
		uint* v = new uint(pStartingValue);
		value = v;
		type = VARIABLE_TYPE_INT;
	}
	ConsoleVariable(string_static pStartingValue) {
		value = SDL_malloc(sizeof(pStartingValue));
		SDL_strlcpy((char*)value, pStartingValue, sizeof(pStartingValue));
		type = VARIABLE_TYPE_STRING;
	}
	ConsoleVariable(bool pStartingValue, bool compilerIssueWeArentAnInt) {
		value = SDL_malloc(1);
		bool* v = new bool(pStartingValue);
		value = v;
		type = VARIABLE_TYPE_BOOL;
	}
};


struct ConsoleCommand {
	string_static mConsoleName = nullptr;
	void (*mExecute)(ConsoleVariable& args) = nullptr;

	ConsoleCommand(void (*pExecute)(ConsoleVariable& args) ) {
		mExecute = pExecute;
	}
};

#include <vector>
class ConsoleHandler {
private:
	ConsoleCommand mCommands[67];
	byte mUsedCommands = 0;
	ConsoleVariable mVariables[67];
	byte mUsedVariables;
public:

	void ExecuteByString(string_static value) {
		char temp[32] = { '\0' };

		std::vector<ConsoleVariable> args;
		char commandStr[32] = {'\0'};
		for (ushort i = 0; i < SDL_strlen(value); i++) {
			if (value[i] == ' ') {
				if (commandStr[0] == '\0') {
					SDL_strlcpy(commandStr, temp, 32);
				}
				else {

					//Find out type
					if (SDL_isdigit(temp[0])) {
						args.push_back(ConsoleVariable(atoi(temp)));
					}
					if (!SDL_strcmp(temp, "false")) {

					}
					SDL_strlcpy((char*)args.at(args.size() - 1).value, temp, 32);
				}
				continue;
			}


			temp[i] = value[i];
		}


	}
};