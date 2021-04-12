#pragma once
#include <windows.h>

#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#endif

#include "Descriptors.h"

namespace GraphicsModule
{
class SwapChain
{
public:
	SwapChain() = default;
	~SwapChain() = default;


#if defined(DX11)

	IDXGISwapChain*& GetSwapChainPtr();

	HRESULT GetBuffer(unsigned int Buffer, REFIID riid, void** ppSurface);
	HRESULT ResizeBuffers(unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags);
	//HRESULT Resize(unsigned int width, unsigned int height);
	void Present(unsigned int SyncInterval, unsigned int Flags);
#endif
	void Release();

private:
#if defined(DX11)
	IDXGISwapChain* m_pSwapChain = NULL;
#endif
};
}
