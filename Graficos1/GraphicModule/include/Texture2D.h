#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

class Texture2D
{
public:
	Texture2D() = default;
	~Texture2D() = default;

#if defined(DX11)

	inline ID3D11Texture2D*& GetTexturePtr() { return m_texture; }
	inline void Release() { if (m_texture) m_texture->Release(); }

private:
	ID3D11Texture2D* m_texture;

#endif
};

