#pragma once
#include "Texture.h"

#include <string>
#include <map>
using std::map;
using std::string;

namespace GraphicsModule
{
class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	static Texture& GetTexture(string name);

	static bool CreateTextureFromFile(LPCSTR pSrcFile, string name);
#if defined(DX11)
	static bool CreateTextureFromBuffer(Texture2D buffer, string name);
#endif

private:
	static map<string, Texture> m_textures;
};
}
