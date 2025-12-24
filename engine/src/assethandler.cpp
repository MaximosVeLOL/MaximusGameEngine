#include "assethandler.h"


AssetHandler* gAssetHandler = nullptr;



AssetHandler::AssetHandler() {
	gAssetHandler = this;
}