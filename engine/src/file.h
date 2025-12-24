#pragma once


//For file operations
#include "common.h"


enum OperationMode {
	o_read = 0,
	o_write,
	o_append,
	o_all
};

constexpr Uint8 filename_length = 100;

constexpr Uint8 ulong_size = sizeof(ulong);



struct File {
	SDL_IOStream* mCurrent = nullptr;
	SDL_PathInfo mInfo = {SDL_PATHTYPE_NONE, 0, 0, 0, 0};
	bool mExisted = false;
	string_static mFileName = nullptr;

	bool uIsOpen() {
		return mCurrent != nullptr;
	}

	string_static uGetFileName() {
		string_editable returnValue = new char[sizeof(mFileName)];
		returnValue[0] = '!';
		returnValue[1] = '!';
		returnValue[2] = '!';
		for (byte i = 0; i < SDL_strlen(mFileName); i++) {
			if (mFileName[i] == '.') break;
			returnValue[i] = mFileName[i];
		}

		return returnValue;
	}

	string_static uGetFileExtension() {
		string_editable returnValue = new char[sizeof(mFileName)];
		returnValue[0] = '!';
		returnValue[1] = '!';
		returnValue[2] = '!';
		bool use = false;
		for (byte i = 0; i < SDL_strlen(mFileName); i++) {
			if (use) {
				returnValue[i] = mFileName[i];
			}
			if (mFileName[i] == '.')
				use = true;

		}

		return returnValue;
	}


	void ReadNumber(void* output) {
		if (!mCurrent) {
			ERROR("Failed to read! (file isn't open)");
		}

		byte size = sizeof(output);
		byte readBytes = 0xFF;
		switch (size) {
		case 1: //A byte
			readBytes = SDL_ReadIO(mCurrent, output, 1);
			break;

		case 2: //A short
			readBytes = SDL_ReadIO(mCurrent, output, 2);
			break;

		case 4: //An int
			readBytes = SDL_ReadIO(mCurrent, output, 4);
			break;
		case ulong_size : //A long 
			readBytes = SDL_ReadIO(mCurrent, output, ulong_size);
			break;
		}

		if (readBytes < size) {
			ERROR("Failed to read! (read bytes less than given)");
		}
	}

	void ReadData(void* output) {
		if (!mCurrent) {
			ERROR("Failed to read! (file isn't open)");
		}
		Uint8 readBytes = SDL_ReadIO(mCurrent, output, sizeof(output));
		if (readBytes < sizeof(output)) {
			ERROR("Failed to read! (read bytes less than given)");
		}
	}

	string_static ReadString(uint size) {
		void* returnValue = SDL_malloc(size);
		SDL_ReadIO(mCurrent, returnValue, size);
		return (string_static)returnValue;
	}

	void* ReadAll() {
		if (!mCurrent) {
			SDL_Log("File isn't open!");
			return nullptr;
		}
		void* returnValue = SDL_malloc(mInfo.size);
		Uint8 readBytes = SDL_ReadIO(mCurrent, returnValue, mInfo.size);
		if (readBytes < sizeof(mInfo.size)) {
			SDL_Log("Failed to read! (read bytes less than given)");
			return nullptr;
		}
		return returnValue;
	}

	//These magic numbers are the size of the value.
	void Write(Uint8 value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, &value, 1);
		if (correctly < 1) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}
	void Write(Uint16 value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, &value, 2);
		if (correctly < 2) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}
	void Write(Uint32 value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, &value, 4);
		if (correctly < 4) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}
	void Write(unsigned long value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, &value, ulong_size);
		if (correctly < ulong_size) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}
	void Write(string_static value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, &value, sizeof(value));
		if (correctly < sizeof(value)) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}
	void Write(string_easy value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		string_static cStr = value.c_str();
		size_t correctly = SDL_WriteIO(mCurrent, cStr, sizeof(cStr));
		if (correctly < sizeof(cStr)) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}

	void Write(void* value) {
		if (!mCurrent) {
			ERROR("Failed to write! (file isn't open)");
		}
		size_t correctly = SDL_WriteIO(mCurrent, value, sizeof(value));
		if (correctly < sizeof(value)) {
			ERROR("Failed to write! (bytes written is less than bytes given)");
		}
	}

	void OpenFile(string_static fileName, OperationMode mode) {
		const char* modeStr = nullptr;
		switch (mode) {
		case o_read:
			modeStr = "r";
			break;
		case o_write:
			modeStr = "w";
			break;
		case o_append:
			modeStr = "a";
			break;
		case o_all:
			modeStr = "r+";
			break;
		}
		mExisted = SDL_GetPathInfo(fileName, &mInfo);
		if (!mExisted) {
			SDL_Log("Warning: The file doesn't exist! (%s)", fileName);
		}
		mCurrent = SDL_IOFromFile(fileName, modeStr);
		if (!mCurrent) {
			ERROR("Failed to open file!");
		}
		mFileName = fileName;
	}

	void OpenFileFormatted(OperationMode mode, string_static format, ...) {
		char formatted[filename_length] = "";
		SDL_snprintf(formatted, filename_length, format);
		OpenFile(formatted, mode);
	}

	void Close() {
		SDL_FlushIO(mCurrent); //Takes some time, wastes performance.
		SDL_CloseIO(mCurrent);
	}

	File() {}
	File(string_static format, ...) {
		OpenFileFormatted(o_all, format);
	}
	File(OperationMode mode, string_static format, ...) {
		OpenFileFormatted(mode, format);
	}

	operator SDL_IOStream* () {
		return mCurrent;
	}
};