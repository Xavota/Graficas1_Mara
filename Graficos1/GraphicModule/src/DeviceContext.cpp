#include "DeviceContext.h"

#if defined(DX11)

ID3D11DeviceContext*& DeviceContext::getDeviceContextPtr()
{
	return m_pImmediateContext;
}

void DeviceContext::UpdateSubresource(Buffer pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData, unsigned int SrcRowPitch, unsigned int SrcDepthPitch)
{
	m_pImmediateContext->UpdateSubresource(pDstResource.getBufferPtr(), DstSubresource, reinterpret_cast<const D3D11_BOX*>(pDstBox), pSrcData, SrcRowPitch, SrcDepthPitch);
}

void DeviceContext::DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation)
{
	m_pImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void DeviceContext::OMSetRenderTargets(unsigned int NumViews, RenderTargetView ppRenderTargetViews, DepthStencilView pDepthStencilView)
{
	m_pImmediateContext->OMSetRenderTargets(NumViews, &ppRenderTargetViews.getPtr(), pDepthStencilView.getStencilViewPtr());
}

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT* pViewports)
{
	m_pImmediateContext->RSSetViewports(NumViewports, reinterpret_cast<const D3D11_VIEWPORT*>(pViewports));
}

void DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	m_pImmediateContext->IASetInputLayout(pInputLayout);
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

void DeviceContext::VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_pImmediateContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::VSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_pImmediateContext->VSSetConstantBuffers(StartSlot, NumBuffers, &ppConstantBuffers.getBufferPtr());
}

void DeviceContext::PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_pImmediateContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::PSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_pImmediateContext->PSSetConstantBuffers(StartSlot, NumBuffers, &ppConstantBuffers.getBufferPtr());
}

void DeviceContext::PSSetShaderResources(unsigned int StartSlot, unsigned int NumViews, ShaderResourceView ppShaderResourceViews)
{
	m_pImmediateContext->PSSetShaderResources(StartSlot, NumViews, &ppShaderResourceViews.getPtr());
}

void DeviceContext::PSSetSamplers(unsigned int StartSlot, unsigned int NumSamplers, ID3D11SamplerState* const* ppSamplers)
{
	m_pImmediateContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
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

void DeviceContext::Release()
{
	if (m_pImmediateContext) m_pImmediateContext->Release();
}

#endif