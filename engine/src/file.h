#pragma once


//For file operations
#include "common.h"


enum OperationMode {
	o_read = 0,
	o_write,
	o_append,
	o_all
};

constexpr ushort filename_length = 128; //Paths can get super long...

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
		char* returnValue = new char[filename_length];
		memset(returnValue, '!', filename_length);
		byte returnValue_index = 0;
		short startI = SDL_strlen(mFileName);
		//Find where the directories end, and the filename starts
		for (; startI >= 0; startI--) {
			if (mFileName[startI] == '/' || mFileName[startI] == '\\') {
				SDL_Log("Found ending point! %d", startI);
				startI++;
				break;
			}
		}
		while (mFileName[startI + returnValue_index] != '.') {
			//SDL_Log("Current: %c", mFileName[startI + returnValue_index] );
			returnValue[returnValue_index] = mFileName[startI + returnValue_index];
			returnValue_index++;
		}
		returnValue[returnValue_index] = '\0';
		SDL_Log("uGetFileName: %s", returnValue);
		return returnValue;
	}

	string_static uGetFileExtension() {
		char* returnValue = new char[filename_length];
		memset(returnValue, '!', filename_length);
		bool use = false;
		for (byte i = 0; i < sizeof(mFileName); i++) {
			if (use) {
				returnValue[i] = mFileName[i];
			}
			if (mFileName[i] == '.')
				use = true;

		}

		return returnValue;
	}


	byte ReadByte() {
		if (!mCurrent) {
			SDL_Log("Failed to read! (file isn't open)");
		}
		byte returnValue = 0;
		byte readBytes = SDL_ReadIO(mCurrent, &returnValue, 1);
		if (readBytes != 1) {
			SDL_Log("Failed to read! (read bytes not equal to what needed)");
			return 0;
		}
		return returnValue;
	}
	ushort ReadShort() {
		if (!mCurrent) {
			SDL_Log("Failed to read! (file isn't open)");
		}
		ushort returnValue = 0;
		ushort readushorts = SDL_ReadIO(mCurrent, &returnValue, 2);
		if (readushorts != 2) {
			SDL_Log("Failed to read! (read ushorts not equal to what needed)");
			return 0;
		}
		return returnValue;
	}
	uint ReadInt() {
		if (!mCurrent) {
			SDL_Log("Failed to read! (file isn't open)");
			return 0;
		}
		uint returnValue = 0;
		uint readuints = SDL_ReadIO(mCurrent, &returnValue, 4);
		if (readuints != 4) {
			SDL_Log("Failed to read! (read uints not equal to what needed)");
		}
		return returnValue;
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

	string_static ReadString(ushort length) {
		if (!mCurrent) {
			SDL_Log("Failed to read string! (file isn't open)");
			return "INVALID";
		}
		char *returnValue = new char[length + 1];
		memset(returnValue, '!', length + 1);
		byte readBytes = SDL_ReadIO(mCurrent, returnValue, length);
		if (readBytes != length) {
			SDL_Log("Failed to read string (read bytes not equal to needed)");
			return "INVALID";
		}
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
		//SDL_Log("Opening formatted string...");
		// Use stack buffer sized by filename_length (constexpr)
		char s[filename_length] = {'!'};

		va_list args;
		va_start(args, format);
		// SDL_vsnprintf writes at most filename_length bytes including the null terminator.
		SDL_vsnprintf(s, filename_length, format, args);
		va_end(args);

		SDL_Log("file name: %s", s);
		OpenFile(s, mode);
	}

	void Close() {
		mFileName = nullptr;
		mInfo = { SDL_PATHTYPE_NONE, 0, 0, 0, 0 };
		SDL_FlushIO(mCurrent); //Takes some time, wastes performance.
		SDL_CloseIO(mCurrent);
		mCurrent = nullptr;
	}

	File() {}
	File(string_static format, ...) {
		OpenFileFormatted(o_all, format);
	}
	File(OperationMode mode, string_static format, ...) {
		OpenFileFormatted(mode, format);
	}
	File(string_static mFileName, OperationMode mode) {
		OpenFile(mFileName, mode);
	}

	operator SDL_IOStream* () {
		return mCurrent;
	}

	~File() {
		Close();

	}
};