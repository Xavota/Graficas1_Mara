#pragma once
#include <windows.h>

#include "ShaderResourceView.h"
#include "Texture2D.h"

namespace GraphicsModule
{
class Texture
{
public:
	Texture() = default;
	~Texture() = default;

	bool CreateTextureFromFile(LPCSTR pSrcFile, unsigned int Flags);
#if defined(DX11)
	bool CreateTextureFromBuffer(Texture2D& buffer);

	ShaderResourceView getBuffer() { return m_texture; }
#elif defined(OGL)
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
