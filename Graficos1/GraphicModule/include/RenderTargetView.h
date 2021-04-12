#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class RenderTargetView
{
public:
	RenderTargetView() = default;
	~RenderTargetView() = default;

#if defined(DX11)
	ID3D11RenderTargetView*& getPtr() { return m_renderTargetView; }
#endif           
	void Release() 
	{
#if defined(DX11)
		if (m_renderTargetView) m_renderTargetView->Release(); 
#endif            
	}

private:
#if defined(DX11)    
	ID3D11RenderTargetView* m_renderTargetView;
#endif
};
}
