#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

class DepthStencilView
{
public:
	DepthStencilView() = default;
	~DepthStencilView() = default;

#if defined(DX11)

	ID3D11DepthStencilView*& getStencilViewPtr() { return m_depthStencilView; }
	void Release() { if ( m_depthStencilView ) m_depthStencilView->Release(); }

private:
	ID3D11DepthStencilView* m_depthStencilView;

#endif
};

