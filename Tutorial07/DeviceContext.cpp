#include "DeviceContext.h"

ID3D11DeviceContext*& DeviceContext::getDeviceContextPtr()
{
	return m_pImmediateContext;
}

void DeviceContext::UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, 
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
}

void DeviceContext::IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot,
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

void DeviceContext::ClearState()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
}

void DeviceContext::Release()
{
	if (m_pImmediateContext) m_pImmediateContext->Release();
	delete this;
}
