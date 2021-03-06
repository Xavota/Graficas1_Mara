#include "SwapChain.h"

#if defined(DX11)

namespace GraphicsModule
{
IDXGISwapChain*& SwapChain::GetSwapChainPtr()
{
    return m_pSwapChain;
}

HRESULT SwapChain::GetBuffer(unsigned int Buffer, REFIID riid, void** ppSurface)
{
    return m_pSwapChain->GetBuffer(Buffer, riid, ppSurface);
}

HRESULT SwapChain::ResizeBuffers(unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags)
{
    return m_pSwapChain->ResizeBuffers(BufferCount, Width, Height, static_cast<DXGI_FORMAT>(NewFormat), SwapChainFlags);
}

void SwapChain::Present(unsigned int SyncInterval, unsigned int Flags)
{
    m_pSwapChain->Present(SyncInterval, Flags);
}

void SwapChain::Release()
{
    if (m_pSwapChain) m_pSwapChain->Release();
}

}
#endif