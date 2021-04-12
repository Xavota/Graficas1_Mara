#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class PixelShader
{
public:
	PixelShader() = default;
	~PixelShader() = default;

#if defined(DX11)
	inline ID3D11PixelShader*& getPixelShader() { return m_pixelShader; }
#endif           
	void Release() 
	{ 
#if defined(DX11)
		if (m_pixelShader) m_pixelShader->Release();
#endif           
	}

private:
#if defined(DX11)
	ID3D11PixelShader* m_pixelShader;
#endif
};
}