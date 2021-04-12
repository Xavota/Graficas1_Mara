#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class DepthStencilView
{
public:
	DepthStencilView() = default;
	~DepthStencilView() = default;

#if defined(DX11)
	ID3D11DepthStencilView*& getStencilViewPtr() { return m_depthStencilView; }
#endif
	void Release() 
	{
#if defined(DX11)
		if ( m_depthStencilView ) m_depthStencilView->Release(); 
#endif
	}

private:
#if defined(DX11)
	ID3D11DepthStencilView* m_depthStencilView;
#endif
};
}
