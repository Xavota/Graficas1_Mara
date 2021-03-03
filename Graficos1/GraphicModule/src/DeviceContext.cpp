#include "DeviceContext.h"

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

#include <iostream>
using std::cout;
using std::endl;

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT* pViewports)
{
	cout << pViewports->Width << " " << pViewports->Height << endl;
	m_pImmediateContext->RSSetViewports(NumViewports, reinterpret_cast<const D3D11_VIEWPORT*>(pViewports));
}

void DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	m_pImmediateContext->IASetInputLayout(pInputLayout);
}

/*void DeviceContext::UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, 
								 const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	m_pImmediateContext->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void DeviceContext::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	m_pImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void DeviceContext::OMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)UINT NumViews, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView)
{
	m_pImmediateContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void DeviceContext::RSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)UINT NumViewports, const D3D11_VIEWPORT* pViewports)
{
	m_pImmediateContext->RSSetViewports(NumViewports, pViewports);
}

void DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	m_pImmediateContext->IASetInputLayout(pInputLayout);
}*/

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

/*void DeviceContext::IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot,
									   __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumBuffers, 
									   ID3D11Buffer* const* ppVertexBuffers, 
									   const UINT* pStrides, 
									   const UINT* pOffsets)
{
	m_pImmediateContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void DeviceContext::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	m_pImmediateContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	m_pImmediateContext->IASetPrimitiveTopology(Topology);
}

void DeviceContext::ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
{
	m_pImmediateContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void DeviceContext::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
	m_pImmediateContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

void DeviceContext::VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	m_pImmediateContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::VSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot, 
										 __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, 
										 ID3D11Buffer* const* ppConstantBuffers)
{
	m_pImmediateContext->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void DeviceContext::PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	m_pImmediateContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::PSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot, 
										 __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, 
										 ID3D11Buffer* const* ppConstantBuffers)
{
	m_pImmediateContext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void DeviceContext::PSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot, 
										 __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumViews, 
										 ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	m_pImmediateContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void DeviceContext::PSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)UINT StartSlot, 
								  __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)UINT NumSamplers, 
								  ID3D11SamplerState* const* ppSamplers)
{
	m_pImmediateContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

void DeviceContext::RSSetState(ID3D11RasterizerState* pRasterizerState)
{
	m_pImmediateContext->RSSetState(pRasterizerState);
}*/

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
