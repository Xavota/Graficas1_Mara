#pragma once
#include "ShaderResourceView.h"

#include <string>
#include <map>
using std::map;
using std::string;

class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	static GraphicsModule::ShaderResourceView& GetTexture(string name);

	static void CreateTextureFromFile(LPCSTR pSrcFile, string name);

private:
	static map<string, GraphicsModule::ShaderResourceView> m_textures;
};

