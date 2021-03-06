#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class ShaderResourceView
{
public:
	ShaderResourceView() = default;
	~ShaderResourceView() = default;

#if defined(DX11)

	ID3D11ShaderResourceView*& getPtr() { return m_shaderResourceView; }
	void Release() { if (m_shaderResourceView) m_shaderResourceView->Release(); }

private:
	ID3D11ShaderResourceView* m_shaderResourceView;

#endif
};
}
