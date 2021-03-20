#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class VertexShader
{
public:
	VertexShader() = default;
	~VertexShader() = default;

#if defined(DX11)
	inline ID3D11VertexShader*& getVertexShader() { return m_vertexShader; }
	void Release() { if (m_vertexShader) m_vertexShader->Release(); }
#endif

private:
#if defined(DX11)
	ID3D11VertexShader* m_vertexShader;
#endif
};
}
