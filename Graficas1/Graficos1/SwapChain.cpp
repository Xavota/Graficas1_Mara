#include "SwapChain.h"

IDXGISwapChain*& SwapChain::GetSwapChainPtr()
{
    return m_pSwapChain;
}

HRESULT SwapChain::GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
{
    return m_pSwapChain->GetBuffer(Buffer, riid, ppSurface);
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
