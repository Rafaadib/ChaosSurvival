
#include "Asset_Manager.h"

Asset_Manager::Asset_Manager() {
	//kosong wak
}

Asset_Manager::~Asset_Manager() {
	ClearMapTexture();
}

void Asset_Manager::AddTexture(std::string nametex, const char* filename) {
	//cek texture di load belum
	if (textureMap.find(nametex) == textureMap.end()) {
		Texture2D tex = LoadTexture(filename);

		//biar ga burik
		SetTextureFilter(tex, TEXTURE_FILTER_POINT);

		//simpan le di map
		textureMap[nametex] = tex;

		//hallo cout
		std::cout << "INFO: [ASSET] Texture Loaded : " << nametex << std::endl;

	}
}

Texture2D Asset_Manager::GetTexture(std::string nametex) {
	//cek texture ada ndak wak
	if (textureMap.find(nametex) != textureMap.end()) {
		return textureMap[nametex];
	}

	std::cerr << "WARNING: [ERROR] Texture Not Found : " << nametex << std::endl;

	//biar gak crash
	return { 0 };
}

void Asset_Manager::ClearMapTexture() {
	//loop biar asset ke hapus dari memory le
	//c++ 14
	for (auto const& entrytexure : textureMap) {
		UnloadTexture(entrytexure.second); 
	}
	textureMap.clear();
	std::cout << "INFO: [ASSET] All Texture Unloaded " << std::endl;
}
