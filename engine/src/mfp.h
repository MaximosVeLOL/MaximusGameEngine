#ifndef __MFP_H__
#define __MFP_H__


#include "common.h"// For variable types
#include "file.h" //For file operations
#include <vector> //For resizable arrays

/*	Xx Max's Fast Parser. xX (Start: 9:4? PM)
	
	JSON uses javascript
	But this uses properties.
	[name] : [value]

*/

class MFP_Section {
private:
	std::vector<Property> mData;
public:
	void AddProperty(string_static name, void* data) {
		mData.push_back(Property(name, data));
	}

	void* GetValue(uint pPropertyIndex) {
		return mData.at(pPropertyIndex).value;
	}

	void* GetValue(string_static pPropertyName) {
		for (Property p : mData) {
			if (!SDL_strcmp(p.mName, pPropertyName))
				return p.value;
		}
		return nullptr;
	}

	void Export(string_static pFileName) {
		File output;
		output.OpenFile(pFileName, o_write);
		for (Property p : mData) {
			output.Write(static_cast<ushort>(0xF000)); //Start flag
			output.Write((void*)p.mName);
			output.Write(static_cast<ushort>(0xF001)); //value flag
			output.Write(p.value);
			output.Write(static_cast<ushort>(0xF002)); //End flag
		}
		output.Close();
	}

	void Import(string_static pFileName) {
		File input;
		input.OpenFile(pFileName, o_read);
		
		byte readMode = 0;
		uint detectIndex = 0;
		Property current;
		/*
			0 - check for type (end flag)
			1 - read start (start flag)
			2 - read data (value flag)
		*/

		
		while (SDL_TellIO(input.mCurrent) <= input.mInfo.size) {
			switch (readMode) {
				case 0: //Look for data
					if (input.ReadShort() == 0xF000) {
						readMode = 1;
						detectIndex == SDL_TellIO(input);
						SDL_Log("%d", detectIndex);
					}
				break;

				case 1: //Read name
					if (input.ReadShort() == 0xF001) {
						byte size = SDL_TellIO(input) - detectIndex; //Get the size of the name
						detectIndex = SDL_TellIO(input);
						string_editable name = new char[size]; //Create the name
						SDL_SeekIO(input, detectIndex, SDL_IO_SEEK_SET); //Seek back (we got the size, now use the data)
						for (byte i = 0; i < size; i++) {
							name[i] = static_cast<char>(input.ReadByte()); //Read the name
						}
						current.mName = const_cast<string_static>(name);
						readMode = 2;
					}
				break;

				case 2: //Read value
					if (input.ReadShort() == 0xF002) {
						byte size = SDL_TellIO(input) - detectIndex; //Get the size of the value
						byte *data = new byte[size];
						//current.value = SDL_malloc(size);
						SDL_SeekIO(input, detectIndex, SDL_IO_SEEK_SET); //Seek back (we got the size, now use the data)
						for (byte i = 0; i < size; i++) {
							data[i] = input.ReadByte();
						}
						current.value = (void *)data;
						readMode = 0;
					}
				break;
			}
		}
	}
};

#endif