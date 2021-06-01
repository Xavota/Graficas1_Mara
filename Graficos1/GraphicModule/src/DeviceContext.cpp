#include "DeviceContext.h"

namespace GraphicsModule
{
#if defined(DX11)
ID3D11DeviceContext*& DeviceContext::getDeviceContextPtr()
{
	return m_pImmediateContext;
}

void DeviceContext::UpdateSubresource(Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData, unsigned int SrcRowPitch, unsigned int SrcDepthPitch)
{
	m_pImmediateContext->UpdateSubresource(pDstResource.getBufferPtr(), DstSubresource, reinterpret_cast<const D3D11_BOX*>(pDstBox), pSrcData, SrcRowPitch, SrcDepthPitch);
}

void DeviceContext::UpdateTexture2D(Texture2D& texture, const void* data, unsigned int rowSize)
{
	m_pImmediateContext->UpdateSubresource(texture.GetTexturePtr(), 0, NULL, data, rowSize, 0);
}

void DeviceContext::DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation)
{
	m_pImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void DeviceContext::OMSetRenderTargets(unsigned int count, RenderTargetView* ppRenderTargetViews, DepthStencilView pDepthStencilView)
{
	std::vector<ID3D11RenderTargetView*> renderTargets;
	for (int i = 0; i < count; i++)
	{
		renderTargets.push_back(ppRenderTargetViews[i].getPtr());
	}
	m_pImmediateContext->OMSetRenderTargets(count, renderTargets.data(), pDepthStencilView.getStencilViewPtr());
}

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT* pViewports)
{
	m_pImmediateContext->RSSetViewports(NumViewports, reinterpret_cast<const D3D11_VIEWPORT*>(pViewports));
}

void DeviceContext::IASetInputLayout(InputLayout& pInputLayout)
{
	m_pImmediateContext->IASetInputLayout(pInputLayout.getInputLayout());
}

void DeviceContext::IASetVertexBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppVertexBuffers, const unsigned int* pStrides, const unsigned int* pOffsets)
{
	m_pImmediateContext->IASetVertexBuffers(StartSlot, NumBuffers, &ppVertexBuffers.getBufferPtr(), pStrides, pOffsets);
}

void DeviceContext::IASetIndexBuffer(Buffer pIndexBuffer, FORMAT Format, unsigned int Offset)
{
	m_pImmediateContext->IASetIndexBuffer(pIndexBuffer.getBufferPtr(), static_cast<DXGI_FORMAT>(Format), Offset);
}

void DeviceContext::IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY Topology)
{
	m_pImmediateContext->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(Topology));
}

void DeviceContext::ClearRenderTargetView(RenderTargetView pRenderTargetView, const float ColorRGBA[4])
{
	m_pImmediateContext->ClearRenderTargetView(pRenderTargetView.getPtr(), ColorRGBA);
}

void DeviceContext::ClearDepthStencilView(DepthStencilView pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil)
{
	m_pImmediateContext->ClearDepthStencilView(pDepthStencilView.getStencilViewPtr(), ClearFlags, Depth, Stencil);
}

void DeviceContext::VSSetShader(VertexShader& pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_pImmediateContext->VSSetShader(pVertexShader.getVertexShader(), ppClassInstances, NumClassInstances);
}

void DeviceContext::VSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_pImmediateContext->VSSetConstantBuffers(StartSlot, NumBuffers, &ppConstantBuffers.getBufferPtr());
}

void DeviceContext::PSSetShader(PixelShader pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_pImmediateContext->PSSetShader(pPixelShader.getPixelShader(), ppClassInstances, NumClassInstances);
}

void DeviceContext::PSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_pImmediateContext->PSSetConstantBuffers(StartSlot, NumBuffers, &ppConstantBuffers.getBufferPtr());
}

void DeviceContext::PSSetShaderResources(unsigned int StartSlot, std::vector<Texture> ppShaderResourceViews)
{
	std::vector<ShaderResourceView> srvs;
	for (Texture& t : ppShaderResourceViews)
	{
		srvs.push_back(t.getBuffer());
	}

	m_pImmediateContext->PSSetShaderResources(StartSlot, srvs.size(), &srvs.data()->getPtr());
}

void DeviceContext::PSSetSamplers(unsigned int StartSlot, unsigned int NumSamplers, SamplerState& ppSamplers)
{
	m_pImmediateContext->PSSetSamplers(StartSlot, NumSamplers, &ppSamplers.getSamplerState());
}

void DeviceContext::RSSetState(ID3D11RasterizerState* pRasterizerState)
{
	m_pImmediateContext->RSSetState(pRasterizerState);
}

void DeviceContext::Flush()
{
	m_pImmediateContext->Flush();
}

void DeviceContext::ClearState()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
}
#endif

void DeviceContext::Release()
{
#if defined(DX11)
	if (m_pImmediateContext) m_pImmediateContext->Release();
#endif
}
}
