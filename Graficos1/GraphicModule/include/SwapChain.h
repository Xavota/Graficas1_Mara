#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include "Descriptors.h"

class SwapChain
{
public:
	SwapChain() = default;
	~SwapChain() = default;


	IDXGISwapChain*& GetSwapChainPtr();

	HRESULT GetBuffer(unsigned int Buffer, REFIID riid, void** ppSurface);
	HRESULT ResizeBuffers(unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags);
	//HRESULT Resize(unsigned int width, unsigned int height);
	void Present(unsigned int SyncInterval, unsigned int Flags);

	//HRESULT GetBuffer(
	//	/* [in] */ UINT Buffer,
	//	/* [annotation][in] */
	//	__in  REFIID riid,
	//	/* [annotation][out][in] */
	//	__out  void** ppSurface);

	//HRESULT ResizeBuffers(
	//	/* [in] */ UINT BufferCount,
	//	/* [in] */ UINT Width,
	//	/* [in] */ UINT Height,
	//	/* [in] */ DXGI_FORMAT NewFormat,
	//	/* [in] */ UINT SwapChainFlags);

	//HRESULT Resize(unsigned int width, unsigned int height);

	//void Present(
	//	/* [in] */ UINT SyncInterval,
	//	/* [in] */ UINT Flags);

	void Release();

private:
	IDXGISwapChain* m_pSwapChain = NULL;
};

