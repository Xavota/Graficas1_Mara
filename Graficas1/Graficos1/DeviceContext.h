#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

class DeviceContext
{
public:
	DeviceContext() = default;
	~DeviceContext() = default;

	ID3D11DeviceContext*& getDeviceContextPtr();

	void UpdateSubresource(
		/* [annotation] */
		__in  ID3D11Resource* pDstResource,
		/* [annotation] */
		__in  UINT DstSubresource,
		/* [annotation] */
		__in_opt  const D3D11_BOX* pDstBox,
		/* [annotation] */
		__in  const void* pSrcData,
		/* [annotation] */
		__in  UINT SrcRowPitch,
		/* [annotation] */
		__in  UINT SrcDepthPitch);

	void DrawIndexed(
		/* [annotation] */
		__in  UINT IndexCount,
		/* [annotation] */
		__in  UINT StartIndexLocation,
		/* [annotation] */
		__in  INT BaseVertexLocation);

	void OMSetRenderTargets(
		/* [annotation] */
		__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
		/* [annotation] */
		__in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
		/* [annotation] */
		__in_opt  ID3D11DepthStencilView* pDepthStencilView);
	void RSSetViewports(
		/* [annotation] */
		__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
		/* [annotation] */
		__in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);
	void IASetInputLayout(
		/* [annotation] */
		__in_opt  ID3D11InputLayout* pInputLayout);

	void IASetVertexBuffers(
		/* [annotation] */
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  const UINT* pStrides,
		/* [annotation] */
		__in_ecount(NumBuffers)  const UINT* pOffsets);
	void IASetIndexBuffer(
		/* [annotation] */
		__in_opt  ID3D11Buffer* pIndexBuffer,
		/* [annotation] */
		__in  DXGI_FORMAT Format,
		/* [annotation] */
		__in  UINT Offset);

	void IASetPrimitiveTopology(
		/* [annotation] */
		__in  D3D11_PRIMITIVE_TOPOLOGY Topology);

	void ClearRenderTargetView(
		/* [annotation] */
		__in  ID3D11RenderTargetView* pRenderTargetView,
		/* [annotation] */
		__in  const FLOAT ColorRGBA[4]);

	void ClearDepthStencilView(
		/* [annotation] */
		__in  ID3D11DepthStencilView* pDepthStencilView,
		/* [annotation] */
		__in  UINT ClearFlags,
		/* [annotation] */
		__in  FLOAT Depth,
		/* [annotation] */
		__in  UINT8 Stencil);

	void VSSetShader(
		/* [annotation] */
		__in_opt  ID3D11VertexShader* pVertexShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);
	void VSSetConstantBuffers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);
	void PSSetShader(
		/* [annotation] */
		__in_opt  ID3D11PixelShader* pPixelShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);
	void PSSetConstantBuffers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);
	void PSSetShaderResources(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		__in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews);
	void PSSetSamplers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		__in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);

	void RSSetState(
		/* [annotation] */
		__in_opt  ID3D11RasterizerState* pRasterizerState);

	void Flush();

	void ClearState();
	void Release();

private:
	ID3D11DeviceContext* m_pImmediateContext = NULL;
};

