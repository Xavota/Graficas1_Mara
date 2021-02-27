#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

class RenderTargetView
{
public:
	RenderTargetView() = default;
	~RenderTargetView() = default;

	ID3D11RenderTargetView*& getPtr() { return m_renderTargetView; }
	void Release() { if (m_renderTargetView) m_renderTargetView->Release(); }

private:
	ID3D11RenderTargetView* m_renderTargetView;
};

