#pragma once

#if defined(DX11)
#include <d3d11.h>
#elif defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif

namespace GraphicsModule
{
class RenderTargetView
{
public:
	RenderTargetView() = default;
	//RenderTargetView(const RenderTargetView& other);
	~RenderTargetView() = default;

#if defined(DX11)
	ID3D11RenderTargetView*& getPtr() { return m_renderTargetView; }
#elif defined(OGL)
	void Init(unsigned int textureAtchID, unsigned int id = 1234);
	unsigned int getID();
	unsigned int getTextureAtatchedID();
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
#elif defined(OGL)
	unsigned int m_frameBuffer;
	unsigned int m_textureAtatchedID = 0;
#endif
};
}
