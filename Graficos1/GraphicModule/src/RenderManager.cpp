#include "RenderManager.h"
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Device*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace GraphicsModule
{
#if defined(DX11)

HRESULT RenderManager::CreateRenderTargetView(Texture2D& texture, const RENDER_TARGET_VIEW_DESC* desc, RenderTargetView& rtv)
{
	return m_device.CreateRenderTargetView(texture.GetTexturePtr(), reinterpret_cast<const D3D11_RENDER_TARGET_VIEW_DESC*>(desc), &rtv.getPtr());
}

HRESULT RenderManager::CreateTexture2D(const TEXTURE2D_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Texture2D& ppTexture2D)
{
	return m_device.CreateTexture2D(reinterpret_cast<const D3D11_TEXTURE2D_DESC*>(pDesc), reinterpret_cast<const D3D11_SUBRESOURCE_DATA*>(pInitialData), &ppTexture2D.GetTexturePtr());
}

HRESULT RenderManager::CreateDepthStencilView(Texture2D& pResource, const DEPTH_STENCIL_VIEW_DESC* pDesc, DepthStencilView& ppDepthStencilView)
{
	return m_device.CreateDepthStencilView(pResource.GetTexturePtr(), reinterpret_cast<const D3D11_DEPTH_STENCIL_VIEW_DESC*>(pDesc), &ppDepthStencilView.getStencilViewPtr());
}

HRESULT RenderManager::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
	return m_device.CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT RenderManager::CreateInputLayout(const INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	return m_device.CreateInputLayout(reinterpret_cast<const D3D11_INPUT_ELEMENT_DESC*>(pInputElementDescs), NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

HRESULT RenderManager::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
	return m_device.CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT RenderManager::CreateBuffer(const BUFFER_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Buffer& ppBuffer)
{
	return m_device.CreateBuffer(reinterpret_cast<const D3D11_BUFFER_DESC*>(pDesc), reinterpret_cast<const D3D11_SUBRESOURCE_DATA*>(pInitialData), &ppBuffer.getBufferPtr());
}

HRESULT RenderManager::CreateSamplerState(const SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	return m_device.CreateSamplerState(reinterpret_cast<const D3D11_SAMPLER_DESC*>(pSamplerDesc), ppSamplerState);
}

HRESULT RenderManager::CreateRasterizerState(const RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
{
	return m_device.CreateRasterizerState(reinterpret_cast<const D3D11_RASTERIZER_DESC*>(pRasterizerDesc), ppRasterizerState);
}

HRESULT RenderManager::CreateShaderResourceView(Texture2D& pResource, const SHADER_RESOURCE_VIEW_DESC* pDesc, ShaderResourceView& ppSRView)
{
	return m_device.CreateShaderResourceView(pResource.GetTexturePtr(), reinterpret_cast<const D3D11_SHADER_RESOURCE_VIEW_DESC*>(pDesc), &ppSRView.getPtr());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Device context*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderManager::DrawObject(OBJInstance* obj)
{
	UINT stride = sizeof(Vertex);

	IASetVertexBuffers(0, 1, obj->getMesh()->getVertexBuffer(), &stride, &offset);
	IASetIndexBuffer(obj->getMesh()->getIndexBuffer(), FORMAT_R16_UINT, 0);

	CBChangesEveryFrame cb;

	cb.mWorld =  obj->getModelMatrix();
	Color col = obj->getMesh()->getColor();
	cb.vMeshColor = *reinterpret_cast<XMFLOAT4*>(&col);
	PSSetShaderResources(0, 1, obj->getTexture());
	UpdateSubresource(m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	DrawIndexed(obj->getMesh()->getIndexCount(), 0, 0);
}

void RenderManager::UpdateSubresource(Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData, unsigned int SrcRowPitch, unsigned int SrcDepthPitch)
{
	m_deviceContext.UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void RenderManager::DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation)
{
	m_deviceContext.DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void RenderManager::OMSetRenderTargets(unsigned int NumViews, RenderTargetView& ppRenderTargetViews, DepthStencilView& pDepthStencilView)
{
	m_deviceContext.OMSetRenderTargets(NumViews,ppRenderTargetViews, pDepthStencilView);
}

void RenderManager::ClearAndSetRenderTargets(unsigned int NumViews, RenderTargetView& ppRenderTargetViews, DepthStencilView& pDepthStencilView, const float ColorRGBA[4])
{
	ClearRenderTargetView(ppRenderTargetViews, ColorRGBA);
	ClearDepthStencilView(pDepthStencilView, CLEAR_DEPTH, 1.0f, 0);
	OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void RenderManager::RSSetViewports(unsigned int NumViewports, const VIEWPORT* pViewports)
{
	m_deviceContext.RSSetViewports(NumViewports, pViewports);
}

void RenderManager::IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	m_deviceContext.IASetInputLayout(pInputLayout);
}

void RenderManager::IASetVertexBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer& ppVertexBuffers, const unsigned int* pStrides, const unsigned int* pOffsets)
{
	m_deviceContext.IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers,pStrides, pOffsets);
}

void RenderManager::IASetIndexBuffer(Buffer& pIndexBuffer, FORMAT Format, unsigned int Offset)
{
	m_deviceContext.IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void RenderManager::IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY Topology)
{
	m_deviceContext.IASetPrimitiveTopology(Topology);
}

void RenderManager::ClearRenderTargetView(RenderTargetView& pRenderTargetView, const float ColorRGBA[4])
{
	m_deviceContext.ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void RenderManager::ClearDepthStencilView(DepthStencilView& pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil)
{
	m_deviceContext.ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

void RenderManager::VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_deviceContext.VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void RenderManager::VSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_deviceContext.VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void RenderManager::PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_deviceContext.PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void RenderManager::PSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_deviceContext.PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void RenderManager::PSSetShaderResources(unsigned int StartSlot, unsigned int NumViews, ShaderResourceView ppShaderResourceViews)
{
	m_deviceContext.PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void RenderManager::PSSetSamplers(unsigned int StartSlot, unsigned int NumSamplers, ID3D11SamplerState* const* ppSamplers)
{
	m_deviceContext.PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

void RenderManager::RSSetState(ID3D11RasterizerState* pRasterizerState)
{
	m_deviceContext.RSSetState(pRasterizerState);
}

void RenderManager::Flush()
{
	m_deviceContext.Flush();
}

void RenderManager::ClearState()
{
	m_deviceContext.ClearState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Swap chain*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT RenderManager::GetBuffer(unsigned int Buffer, REFIID riid, void** ppSurface)
{
	return m_swapChain.GetBuffer(Buffer, riid, ppSurface);
}

HRESULT RenderManager::ResizeBuffers(unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags)
{
	return m_swapChain.ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

void RenderManager::Present(unsigned int SyncInterval, unsigned int Flags)
{
	m_swapChain.Present(SyncInterval, Flags);
}

HRESULT RenderManager::CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, DRIVER_TYPE DriverType, HMODULE Software, unsigned int Flags, 
		const FEATURE_LEVEL* pFeatureLevels, unsigned int FeatureLevels, unsigned int SDKVersion, const SWAP_CHAIN_DESC* pSwapChainDesc, 
		FEATURE_LEVEL* pFeatureLevel)
{
	return D3D11CreateDeviceAndSwapChain(pAdapter, static_cast<D3D_DRIVER_TYPE>(DriverType), Software, Flags, 
		reinterpret_cast<const D3D_FEATURE_LEVEL*>(pFeatureLevels), FeatureLevels, SDKVersion, 
		reinterpret_cast<const DXGI_SWAP_CHAIN_DESC*>(pSwapChainDesc), &m_swapChain.GetSwapChainPtr(), &m_device.GetDevicePtr(), 
		reinterpret_cast<D3D_FEATURE_LEVEL*>(pFeatureLevel), &m_deviceContext.getDeviceContextPtr());
}

HRESULT RenderManager::CreateShaderResourceViewFromFile(LPCSTR pSrcFile, D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump, ShaderResourceView& ppShaderResourceView, HRESULT* pHResult)
{
	return D3DX11CreateShaderResourceViewFromFile(m_device.GetDevicePtr(), pSrcFile, pLoadInfo, pPump, &ppShaderResourceView.getPtr(), pHResult);
}

HRESULT RenderManager::CreateShaderAsRenderTargetView(ShaderResourceView& ViewRT, RenderTargetView& RenderTargetView,
	unsigned int width, unsigned int height)
{
	Texture2D TextRT;
	
	HRESULT hr;

	TEXTURE2D_DESC descTextRT;
	ZeroMemory(&descTextRT, sizeof(descTextRT));
	descTextRT.Width = width;
	descTextRT.Height = height;
	descTextRT.MipLevels = 1;
	descTextRT.ArraySize = 1;
	descTextRT.Format = FORMAT_R8G8B8A8_UNORM;
	descTextRT.SampleDesc.Count = 1;
	descTextRT.SampleDesc.Quality = 0;
	descTextRT.Usage = USAGE_DEFAULT;
	descTextRT.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
	descTextRT.CPUAccessFlags = 0;
	descTextRT.MiscFlags = 0;
	hr = CreateTexture2D(&descTextRT, NULL, TextRT);
	if (FAILED(hr))
		return hr;

	// create the rt Shader resource view 2
	SHADER_RESOURCE_VIEW_DESC descViewRT;
	ZeroMemory(&descViewRT, sizeof(descViewRT));
	descViewRT.Format = FORMAT_R8G8B8A8_UNORM;
	descViewRT.ViewDimension = SRV_DIMENSION_TEXTURE2D;
	descViewRT.Texture2D.MostDetailedMip = 0;
	descViewRT.Texture2D.MipLevels = 1;
	hr = CreateShaderResourceView(TextRT, &descViewRT, ViewRT);
	if (FAILED(hr))
		return hr;

	// Create the render target view 2
	hr = CreateRenderTargetView(TextRT, NULL, RenderTargetView);
	if (FAILED(hr))
		return hr;

	TextRT.Release();
}

void RenderManager::UpdateViewMatrix(MATRIX view)
{
	XMMATRIX g_View = *reinterpret_cast<XMMATRIX*>(&view);
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_View);
	UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
}

void RenderManager::UpdateProjectionMatrix(MATRIX projection)
{
	XMMATRIX g_Projection = *reinterpret_cast<XMMATRIX*>(&projection);
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
	UpdateSubresource(m_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
}

void RenderManager::UpdateWorld(MATRIX model, Color color)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Release*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderManager::Release()
{
	m_device.Release();
	m_deviceContext.Release();
	m_swapChain.Release();

	m_pCBNeverChanges.Release();
	m_pCBChangeOnResize.Release();
	m_pCBChangesEveryFrame.Release();
}
#endif

extern RenderManager* GetManager()
{
	static RenderManager* manager = nullptr;
	if (nullptr == manager)
	{
		manager = new RenderManager();
	}
	return manager;
}
}