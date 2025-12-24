#ifndef __ASSET_HANDLER_H__
#define __ASSET_HANDLER_H__


#include "common.h"

constexpr ushort MAX_ASSETS_LOADED = 20;


enum AssetType : byte {
	ASSET_TYPE_TEXTURE = 0, //Uses SDL_LoadPNG, SDL_LoadBMP, ect.
	ASSET_TYPE_FILE, //Uses regular files
	ASSET_TYPE_AUDIO, //Uses mixer functions
	ASSET_TYPE_SPRITE, //Uses the sprite system
};

enum AssetGroupID : byte {
	ASSET_GROUP_TEST = 0,
	ASSET_GROUP_LAST,
};

struct Asset {
	AssetType mType = ASSET_TYPE_FILE;
	string_static mID = nullptr;
	string_static mFileName = nullptr;
	void* mData = nullptr;


	bool TryLoad() {
		switch (mType) {
		case ASSET_TYPE_TEXTURE:
			if (SDL_strstr(mFileName, ".bmp")) {
				mData = SDL_LoadBMP(mFileName);
			}
			else if (SDL_strstr(mFileName, ".png")) {
				mData = SDL_LoadPNG(mFileName);
			}
			break;

		case ASSET_TYPE_FILE: {
			SDL_IOStream* temp = SDL_IOFromFile(mFileName, "r");
			if (!temp) return false;
			Sint64 fileSize = 0;
			SDL_SeekIO(temp, 0, SDL_IO_SEEK_END);
			fileSize = SDL_TellIO(temp);
			if (fileSize < 0) return false;
			SDL_SeekIO(temp, 0, SDL_IO_SEEK_SET);
			
			mData = SDL_malloc(fileSize);
			SDL_ReadIO(temp, mData, fileSize);

			}
			break;

		case ASSET_TYPE_AUDIO:
			break;
		}


		return mData != nullptr;
	}
};

// Fixed-size array used here so each AssetGroup instance has its own storage
struct AssetGroup {
	ushort mLoadedAssets = 0;
	Asset mAssets[MAX_ASSETS_LOADED];
};

const AssetGroup mGroups[ASSET_GROUP_LAST] = {
	AssetGroup(),
};

class AssetHandler {
private:

public:
	AssetHandler();

	// Load all valid assets in the group. Returns true if at least one asset loaded.
	bool LoadGroup(AssetGroupID pAssetGroup) {
		if (pAssetGroup >= ASSET_GROUP_LAST) return false;
		AssetGroup group = mGroups[pAssetGroup];
		group.mLoadedAssets = 0;
		for (ushort i = 0; i < MAX_ASSETS_LOADED; ++i) {
			Asset& asset = group.mAssets[i];
			// Skip empty slots (assumes string_static is nullptr when unused)
			if (!asset.mFileName) continue;
			if (asset.TryLoad()) {
				++group.mLoadedAssets;
			}
		}
		return group.mLoadedAssets > 0;
	}
};


extern AssetHandler* gAssetHandler;


#endif