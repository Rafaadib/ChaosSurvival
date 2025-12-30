
#pragma once

#include <raylib.h>

#include <iostream>
#include <map>
#include <string>

class Asset_Manager {
public:
	Asset_Manager();
	~Asset_Manager();

	//fungsi untk muat trexture
	void AddTexture(std::string nametex, const char* filename);

	Texture2D GetTexture(std::string nametex);

	//bersihkan le
	void ClearMapTexture();

private:
	//gunain mapp untuk data terurut
	//karena texture adalah data terurut:)
	std::map<std::string, Texture2D> textureMap;

};
