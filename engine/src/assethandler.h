
#ifndef __ASSET_HANDLER_H__
#define __ASSET_HANDLER_H__

/*	Asset System
* How are we going to load assets?
*	We are going to load them by requesting an asset ID (or filename),
	and then loading it through the asset's file name,
	and then returning the data from the file (that is parsed)
* How can we implement this?
*	Have a const asset group array that has all created assets in it.
*	OR
*	Have a const struct that has all the asset groups in it and assets.
* What happens when errors occur?
*	This will be easy to fix. We will replace the missing assets with placeholders, in order to fix it.

*/

#include "common.h" //For typedefs, and gEzRender.

#if COMOPT_A_USE

#include "file.h" //For file operations
#include "audiosystem.h" //Sounds
#include "ezrender.h" //For mRenderer
#include "sprite.h" //For sprites
#include <vector> //For the loaded assets/asset groups

extern string_static uGetRootDirectory();
extern string_static uGetGraphicsDirectory();

/*
enum AssetGroupID : byte {
	ASSET_GROUP_NONE = 0,
	ASSET_GROUP_TEST,
	ASSET_GROUP_LAST,
};
*/

extern bool uFileExtensionEquals(string_static pFileName, string_static pTargetExtension);
extern void LoadResources();

struct DefaultAssets {
	SDL_Surface* mTexture = nullptr;
#if COMOPT_S_USE
	MIX_Audio* mAudio = nullptr;
#endif
	Sprite* mSprite = nullptr;

};

extern DefaultAssets mDefault;

//I am so tired of having to base things off of its file extension, and what if there are other file extensions? (Check for it also, bruh...)
//So I am using this, because there are many use cases where we need the asset type.
enum AssetType : byte {
	ASSET_TYPE_AUDIO = 0,
	ASSET_TYPE_IMAGE = 1,
	ASSET_TYPE_SPRITE = 2,
	ASSET_TYPE_FILE = 3,
	ASSET_TYPE_TILESET = 4,
};


struct Asset {
	char mFileName[32] = {'\0'};
	//string_static mFileName = nullptr;
	void* mData = nullptr;
	AssetType mType = ASSET_TYPE_FILE;
	//bool mFailedLoad = false;

	void LoadDefault() {
		switch (mType) {
#if COMOPT_S_USE
		case ASSET_TYPE_AUDIO:
			mData = mDefault.mAudio;
			break;
#endif
		case ASSET_TYPE_IMAGE:
			mData = mDefault.mTexture;
			break;
		case ASSET_TYPE_SPRITE:
			mData = mDefault.mSprite;
			break;
		case ASSET_TYPE_FILE:
			mData = nullptr;
			break;
		case ASSET_TYPE_TILESET:
			break;
		}
	}

	bool TryLoad() {
		if (!mFileName) {
			SDL_Log("Our file name is nullptr!");
			return false;
		}
		if (mData) {
			SDL_Log("Our data is already loaded!");
			return true;
		}


		//SDL_Log("mFileName: %s", mFileName);
		SDL_Log("Trying to load file! (filename: %s/%s)", uGetGraphicsDirectory(), mFileName);
		char s[filename_length];
		SDL_snprintf(s, 128, "%s/%s", uGetGraphicsDirectory(), mFileName);
		File file; //Don't use initializer, it breaks everything.
		file.OpenFile(s, o_read);
		if (!file.uIsOpen()) {
			SDL_Log("Failed to load!");
			LoadDefault();
			return false;
		}
		switch (mType) {
#if COMOPT_S_USE
		case ASSET_TYPE_AUDIO:
			mData = new MIX_Audio*;
			mData = (void*)MIX_LoadAudio_IO(gAudio->mMixer, file.mCurrent, true, false);
			if (!mData) { LoadDefault(); return false; }
			break;
#endif
		case ASSET_TYPE_IMAGE:
			mData = SDL_LoadPNG_IO(file.mCurrent, false);
			if (!mData) { LoadDefault(); return false; }

			break;
		case ASSET_TYPE_SPRITE: {

			mData = new Sprite();
			Sprite& asSprite = *(Sprite*)mData;

			//Load sprite file
			string_static header = file.ReadString(4);
			SDL_Log("Header: %s", header);
			if (SDL_strcmp(header, "MESF") != 0 && false) {
				SDL_Log("Invalid header!");
				LoadDefault();
				return false;
			}
			float cellAmount = (file.mInfo.size - 4) / 6; //Subtract the header and divide by the size of a cell in order to get the amount of cells
			if (cellAmount != SDL_floorf(cellAmount) && false) {
				SDL_Log("Invalid cell amount!");
				LoadDefault();
				return false;
			}
			SDL_Log("Cell amount: %f", cellAmount);

			asSprite.mCells = new Cell[SDL_lround(cellAmount)];
			for (byte i = 0; i < SDL_lround(cellAmount); i++) {
				Cell* cur = &asSprite.mCells[i];
				cur->width = file.ReadShort();
				cur->height = file.ReadShort();
				cur->frame_delay = file.ReadByte();
				cur->frame_amount = file.ReadByte();
				SDL_Log("Cell data (ind: %d):\nwidth: %d\nheight: %d\nframe delay: %d\nframe amount: %d", i, cur->width, cur->height, cur->frame_delay, cur->frame_amount);

			}
			string_static fileName = file.uGetFileName(); //Get the file name (sprite.MESF -> sprite)
			file.Close();
			//Load image

			file.OpenFileFormatted(o_read, "%s/%s.png", uGetGraphicsDirectory(), fileName); //(sprite.MESF -> sprite -> sprite.png)
			if (!file.uIsOpen()) {
				LoadDefault();
				return false;
			}
			SDL_Surface* image = SDL_LoadPNG_IO(file, false);
			if (!image) {
				LoadDefault();
				return false;
			}
#if COMOPT_R_USE_HA
			asSprite.mTexturePage = SDL_CreateTextureFromSurface(gEzRender->mRenderer, image);
#else
			asSprite.mTexturePage = image;
#endif
			if (!asSprite.mTexturePage) {
				LoadDefault();
				return false;
			}
			SDL_DestroySurface(image);
		}
			break;
		case ASSET_TYPE_FILE:
			mData = file.ReadAll();
			break;
		case ASSET_TYPE_TILESET:
			break;
		}
		SDL_Log("Loaded successfully!");
		return true;
	}

	Asset() {}
};

class AssetGroup {
public:
	char mName[32] = {'\0'};
	ushort mLoadedAssets = 0;
	std::vector<Asset> mAssets;

};


extern std::vector<AssetGroup> mGroups;

extern short uGetGroupIndexByName(string_static name);

class AssetHandler {
private:
	//AssetGroupID mCurrentGroup = ASSET_GROUP_NONE;
	short mCurrentGroup = -1;
	ushort mTotalAssetsLoaded = 0;
	uint mMemoryUsage = 0;
public:



	void UnloadGroup(short pGroupIndex = -1) {
		if (pGroupIndex == -1) {
			pGroupIndex = mCurrentGroup;
		}

		AssetGroup g = mGroups[pGroupIndex];
		for (ushort i = 0; i < g.mLoadedAssets; i++) {
			SDL_free(g.mAssets[i].mData);
		}
		g.mLoadedAssets = 0;
	}

	void UnloadGroup(string_static pGroupName = "invalid") {
		short pGroupIndex = uGetGroupIndexByName(pGroupName);
		if (pGroupIndex == -1) {
			pGroupIndex = mCurrentGroup;
		}

		AssetGroup g = mGroups[pGroupIndex];
		for (ushort i = 0; i < g.mLoadedAssets; i++) {
			SDL_free(g.mAssets[i].mData);
		}
		g.mLoadedAssets = 0;
	}

	void UnloadAllGroups() {
		for (ushort g = 0; g < sizeof(mGroups); g++) {
			UnloadGroup(g);
		}
	}

	void LoadGroupByIndex(ushort pWhichGroup, bool pUnloadOtherGroups = false) {
		if (mCurrentGroup >= 0 && pUnloadOtherGroups) {
			UnloadGroup(mCurrentGroup);
		}

		SDL_Log("Loading group...");
		mCurrentGroup = pWhichGroup;

		//SDL_Log("Trying to load assets...");
		AssetGroup *g = &mGroups[pWhichGroup];
		for (ushort i = 0; i < g->mAssets.size(); i++) {
			//SDL_Log("Loading asset %d / %d", i + 1, g.mAssets.size());
			if (g->mAssets[i].TryLoad()) {
				g->mLoadedAssets++;
				//SDL_Log("sizeof mAssets[%d]: %d", i, sizeof(g.mAssets[i].mData) );
				mMemoryUsage += sizeof(g->mAssets[i].mData);
			}
			else {
				SDL_Log("Warning: Failed to load asset for index %d! (filename: %s)", i, g->mAssets[i].mFileName);
			}
		}
		SDL_Log("Loaded group! (memory usage: %d)", mMemoryUsage);


		//return mLoadedAssets >= mAssetCount;
	}

	void LoadGroupByName(string_static pGroupName, bool pUnloadOtherGroups = false) {
		short pWhichGroup = uGetGroupIndexByName(pGroupName);
		if (pWhichGroup == -1) {
			SDL_Log("Failed to find group!");
			return;
		}
		if (mCurrentGroup >= 0 && pUnloadOtherGroups) {
			UnloadGroup(mCurrentGroup);
		}

		SDL_Log("Loading group...");
		mCurrentGroup = pWhichGroup;

		//SDL_Log("Trying to load assets...");
		AssetGroup *g = &mGroups[pWhichGroup];
		for (ushort i = 0; i < g->mAssets.size(); i++) {
			//SDL_Log("Loading asset %d / %d", i + 1, g.mAssets.size());
			if (g->mAssets[i].TryLoad()) {
				g->mLoadedAssets++;
				//SDL_Log("sizeof mAssets[%d]: %d", i, sizeof(g.mAssets[i].mData) );
				mMemoryUsage += sizeof(g->mAssets[i].mData);
			}
			else {
				SDL_Log("Warning: Failed to load asset for index %d! (filename: %s)", i, g->mAssets[i].mFileName);
			}
		}
		SDL_Log("Loaded group! (memory usage: %d)", mMemoryUsage);


		//return mLoadedAssets >= mAssetCount;
	}

	void EnsureGroupIsLoaded(ushort pWhichGroup, bool pUnloadOthers = false) {
		if (mCurrentGroup != pWhichGroup || mGroups[pWhichGroup].mLoadedAssets == 0) {
			LoadGroupByIndex(pWhichGroup, pUnloadOthers);
		}
	}

	AssetGroup* GetCurrentGroup() {
		return &mGroups[mCurrentGroup];
	}

	void* GetAssetData(string_static pFileName) {
		if (mCurrentGroup < 0) {
			SDL_Log("Error: we don't have a current group!");
			return nullptr;
		}
		SDL_Log("Getting asset data...");
		AssetGroup cur = mGroups[mCurrentGroup];
		
		SDL_Log("Target filename data: text : %s\nsize: %d (+1)", pFileName, SDL_strlen(pFileName));
		for (byte i = 0; i < cur.mLoadedAssets; i++) {
			Asset curA = mGroups[mCurrentGroup].mAssets.at(i);
			SDL_Log("Asset %d data: text : %s\nsize: %d (+1)", i, pFileName, SDL_strlen(pFileName));
			if (!SDL_strcmp(curA.mFileName, pFileName))
				return curA.mData;
		}
		return nullptr;
	}

	AssetHandler();
};

extern AssetHandler* gAssetHandler;

#endif //COMOPT_A_USE
#endif //__ASSET_HANDLER_H__