#pragma once
#include <windows.h>

#include "ShaderResourceView.h"
#include "Texture2D.h"

namespace GraphicsModule
{
	enum class eDIMENSION
	{
		NONE = -1,
		TEXTURE2D,
		TEX_CUBE,
		COUNT
	};
class Texture
{
public:
	Texture() = default;
	~Texture()
	{
		//m_texture.Release();
	}

	bool CreateTextureFromFile(LPCSTR pSrcFile, unsigned int Flags, eDIMENSION dim);
#if defined(DX11)
	bool CreateTextureFromBuffer(Texture2D& buffer);

	ShaderResourceView getBuffer() { return m_texture; }
#elif defined(OGL)
	void CreateEmptyTexture(int width, int height);
	unsigned int getID() { return m_ID; }
#endif

private:
#if defined(DX11)
	ShaderResourceView m_texture;
#elif defined(OGL)
	unsigned int m_ID;
#endif
};
}
