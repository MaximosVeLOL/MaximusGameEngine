#include "assethandler.h"


#ifdef COMOPT_A_USE
DefaultAssets mDefault;
void InitializeDefaultTextures() {
	File f;
	f.OpenFileFormatted(o_read, "%s/invalid.png", uGetGraphicsDirectory());
	mDefault.mTexture = SDL_LoadPNG_IO(f.mCurrent, false);
	f.Close();
#if COMOPT_S_USE
	f.OpenFileFormatted(o_read, "%s/invalid.wav", uGetGraphicsDirectory() );
	mDefault.mAudio = MIX_LoadAudio_IO(gAudio->mMixer, f.mCurrent, true, false);
	f.Close();
#endif
	mDefault.mSprite = new Sprite();
#if COMOPT_R_USE_HA
	mDefault.mSprite->mTexturePage = SDL_CreateTextureFromSurface(gEzRender->mRenderer, mDefault.mTexture);
#else
	mDefault.mSprite->mTexturePage = mDefault.mTexture;
#endif
	mDefault.mSprite->mCells = new Cell[1];
	Cell* c = &mDefault.mSprite->mCells[0];
	c->width = 64;
	c->height = 64;
	c->frame_delay = 10;
	c->frame_amount = 2;
}


string_static uGetRootDirectory() {
	return "M:/source/MaximusGameEngine/root";
}

string_static uGetGraphicsDirectory() {	
	string_static rootDir = uGetRootDirectory();
	const ushort size = SDL_strlen(rootDir) + 5;
	string_editable returnValue = new char[size]; //root + "/gfx\0"
	SDL_snprintf(returnValue, size, "%s/gfx", rootDir);
	//SDL_Log("Directory: %s", returnValue);
	//returnValue[size] = '\0';
	return const_cast<string_static>(returnValue);

	/*
	static string_editable returnValue = nullptr;
	if (!returnValue) {
		string_editable curDir = SDL_GetCurrentDirectory();
		ushort size = SDL_strlen(curDir) + 4;
		returnValue = new char[size];
		SDL_snprintf(returnValue, size, "%sgfx", curDir);
		returnValue[SDL_strlen(returnValue) + 1] = '\0';
		//SDL_Log("returnValue: %s", returnValue);
		SDL_free(curDir);
	}

	//memset(returnValue, '!', size);
	


	return returnValue;
	*/
}

bool uFileExtensionEquals(string_static pFileName, string_static pTargetExtension) {
	char* pos_str = SDL_strstr(pFileName, ".");
	if (!pos_str) return false;
	byte pos_real = pos_str - pFileName;
	//SDL_Log("Dot position: %d", pos_real);
	for (byte i = 0; i < SDL_strlen(pTargetExtension); i++) {
		//SDL_Log("(%d) %c = %c", i, pFileName[pos_real + i], pTargetExtension[i]);
		if (pFileName[pos_real + i] != pTargetExtension[i]) return false;
	}

	return true;
}
/*
class TestAssetGroup : public AssetGroup {
public:
	TestAssetGroup() {
		//SDL_Log("Initializing mAssets!");
		mAssets[0] = Asset(ASSET_TYPE_SPRITE, "plr.MESF");
		mAssets[1] = Asset(ASSET_TYPE_AUDIO, "bg.wav");
		//mAssets[1] = Asset("test.MESF");
		mAssetCount = 2;
	}
};

AssetGroup mGroups[ASSET_GROUP_LAST] = {
	AssetGroup(),
	TestAssetGroup(),

};
*/

std::vector<AssetGroup> mGroups;

AssetHandler* gAssetHandler = nullptr;

short uGetGroupIndexByName(string_static name) {
	for (ushort i = 0; i < mGroups.size(); i++) {
		SDL_Log("%s = %s ?", name, mGroups.at(i).mName);
		if (!SDL_strcmp(mGroups.at(i).mName, name)) return i;
	}
	return -1;
}

#include "utility.h"
void LoadResources() {
	File f;
	f.OpenFileFormatted(o_read, "%s/resources.txt", uGetRootDirectory());
	if (!f.uIsOpen()) {
		//return;
		Error(2, "Failed to load resources file!");
	}
	char current = -1;
	char name[64] = { '\0' };
	byte nameIndex = 0;
	byte readMode = 0;
	/* Read types
		0 = none
		1 = reading group name
		2 = reading asset name
	*/

	while (current != -128) {
		current = f.ReadChar();
		if (current == '+' && readMode == 0) {
			current = f.ReadChar();
			while (current != 0x0D) {
				name[nameIndex] = current;
				nameIndex++;
				current = f.ReadChar();
			}
			
			mGroups.push_back(AssetGroup());
			for (ushort i = 0; i < SDL_strlen(name); i++) {
				mGroups[mGroups.size() - 1].mName[i] = name[i];
			}
			SDL_Log("Detected a new asset group! ID: %s", name);
			
			memset(name, '\0', 64);
			nameIndex = 0;
			readMode = 1;
			f.Seek(1); //0D 0A
		}
		if (readMode == 1) {
			current = f.ReadChar();
			while (current != '-') {
				if (current == 0x0D) {
					Asset a;
					
					//Determine asset type
					a.mType = ASSET_TYPE_FILE;
					if (SDL_strstr(name, ".wav")) {
						a.mType = ASSET_TYPE_AUDIO;
					}
					else if (SDL_strstr(name, ".png")) {
						a.mType = ASSET_TYPE_IMAGE;
					}
					else if (SDL_strstr(name, ".MESF")) {
						a.mType = ASSET_TYPE_SPRITE;
					}
					else if (SDL_strstr(name, ".METS")) {
						a.mType = ASSET_TYPE_TILESET;
					}
					/*
					else if (SDL_strstr(name, ".MEFF")) {
						a.mType = ASSET_TYPE_FONT;
					}
					*/
					for (ushort i = 0; i < SDL_strlen(name); i++) {
						a.mFileName[i] = name[i];
					}
					mGroups[mGroups.size() - 1].mAssets.push_back(a);

					SDL_Log("Got new asset for current group!\nFile name: %s\nType: %d", a.mFileName, a.mType);
					nameIndex = 0;
					memset(name, '\0', 64);
					f.Seek(1); //0D 0A
					current = f.ReadChar();
					continue;
				}
				name[nameIndex] = current;
				nameIndex++;
				current = f.ReadChar();
			}

			readMode = 0;
		}

	}
}

AssetHandler::AssetHandler() {
	gAssetHandler = this;
	InitializeDefaultTextures();
	LoadResources();
}


#endif