#pragma once
#include "Texture.h"

#include <string>
#include <map>
using std::map;
using std::string;

namespace GraphicsModule
{
enum class eSTATUS
{
	FAIL = 0,
	REPITED = 1,
	OK = 2
};
class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	static Texture& GetTexture(string name);

	static eSTATUS CreateTextureFromFile(std::vector<string> pSrcFile, string name, unsigned int Flags, eDIMENSION dim);
#if defined(DX11)
	static eSTATUS CreateTextureFromBuffer(Texture2D buffer, string name);
#endif

private:
	static map<string, Texture> m_textures;
};
}
