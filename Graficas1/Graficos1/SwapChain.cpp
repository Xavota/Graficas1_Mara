#include "SwapChain.h"

IDXGISwapChain*& SwapChain::GetSwapChainPtr()
{
    return m_pSwapChain;
}

HRESULT SwapChain::GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
{
    return m_pSwapChain->GetBuffer(Buffer, riid, ppSurface);
}

HRESULT SwapChain::ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    return m_pSwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HRESULT SwapChain::Resize(unsigned int width, unsigned int height)
{
    return E_NOTIMPL;
}

void SwapChain::Release()
{
    if (m_pSwapChain) m_pSwapChain->Release();
    delete this;
}

void SwapChain::Present(UINT SyncInterval, UINT Flags)
{
    m_pSwapChain->Present(SyncInterval, Flags);
}
