#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class Texture2D
{
public:
	Texture2D() = default;
	~Texture2D() = default;

#if defined(DX11)
	inline ID3D11Texture2D*& GetTexturePtr() { return m_texture; }
#endif           
	inline void Release() 
	{
#if defined(DX11)
		if (m_texture) m_texture->Release();
#endif           
	}

private:
#if defined(DX11)
	ID3D11Texture2D* m_texture;
#endif
};
}
