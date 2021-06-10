#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class RasterizeState
{
public:
	RasterizeState() = default;
	~RasterizeState() = default;

#if defined(DX11)
	inline ID3D11RasterizerState*& getRasterizerState() { return m_rasterizeState; }
#endif
	void Release()
	{
#if defined(DX11)
		if (m_rasterizeState) m_rasterizeState->Release();
#endif           
	}

private:
#if defined(DX11)
	ID3D11RasterizerState* m_rasterizeState;
#endif
};
}

