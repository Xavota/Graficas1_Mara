#pragma once
#if defined(DX11)
#include <d3d11.h>
#elif defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
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
#elif defined(OGL)
	void setCullingMode(unsigned int cull);
	unsigned int getCullingMode();
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
#elif defined(OGL)
	unsigned int m_culling = 0;
#endif
};
}

