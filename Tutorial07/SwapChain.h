#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

class SwapChain
{
public:
	SwapChain() = default;
	~SwapChain() = default;

	IDXGISwapChain*& GetSwapChainPtr();

	HRESULT GetBuffer(
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][out][in] */
		__out  void** ppSurface);

	void Release();

	void Present(
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags);

private:
	IDXGISwapChain* m_pSwapChain = NULL;
};

