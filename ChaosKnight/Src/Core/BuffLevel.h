
#pragma once

#include <vector>

#include "Utilities/AmbaBuff.h"
#include "Manager/Asset_Manager.h"


class BuffLevel {
public:

	BuffLevel();

	//tidak boleh statis le
	int UpdateAndChoice(Vector2 MousePosition, const std::vector<BuffDefinition>& optionbuff );

	void DrawBuffLevel(int screenwidth, int screenheight, const std::vector<BuffDefinition>& optionbuff, Asset_Manager* AssetBuff);

};
