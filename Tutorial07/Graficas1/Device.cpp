#include "Device.h"

ID3D11Device*& Device::GetDevicePtr()
{
    return m_pd3dDevice;
}

HRESULT Device::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
    return m_pd3dDevice->CreateRenderTargetView(pResource, pDesc, ppRTView);
}

HRESULT Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, __in_xcount_opt(pDesc -> MipLevels* pDesc -> ArraySize) const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
    return m_pd3dDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
}

HRESULT Device::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
    return m_pd3dDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

HRESULT Device::CreateVertexShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage, __out_opt  ID3D11VertexShader** ppVertexShader)
{
    return m_pd3dDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                                  __in_range(0, D3D11_1_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)UINT NumElements, 
                                  const void* pShaderBytecodeWithInputSignature, 
                                  SIZE_T BytecodeLength, 
                                  ID3D11InputLayout** ppInputLayout)
{
    return m_pd3dDevice->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature,
                                           BytecodeLength, ppInputLayout);
}

HRESULT Device::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
    return m_pd3dDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
    return m_pd3dDevice->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

HRESULT Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
    return m_pd3dDevice->CreateSamplerState(pSamplerDesc, ppSamplerState);
}

void Device::Release()
{
    if (m_pd3dDevice) m_pd3dDevice->Release();
    delete this;
}
