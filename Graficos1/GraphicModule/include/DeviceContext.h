#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include "Descriptors.h"

#include "Texture2D.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "Buffer.h"

class DeviceContext
{
public:
	DeviceContext() = default;
	~DeviceContext() = default;

	ID3D11DeviceContext*& getDeviceContextPtr();


	void UpdateSubresource(Buffer pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData,
		unsigned int SrcRowPitch, unsigned int SrcDepthPitch);
	void DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation);
	void OMSetRenderTargets(unsigned int NumViews, RenderTargetView ppRenderTargetViews, DepthStencilView pDepthStencilView);
	void RSSetViewports(unsigned int NumViewports, const VIEWPORT* pViewports);
	void IASetInputLayout(ID3D11InputLayout* pInputLayout);
	void IASetVertexBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppVertexBuffers,
		const unsigned int* pStrides, const unsigned int* pOffsets);
	void IASetIndexBuffer(Buffer pIndexBuffer, FORMAT Format, unsigned int Offset);
	void IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY Topology);
	void ClearRenderTargetView(RenderTargetView pRenderTargetView, const float ColorRGBA[4]);
	void ClearDepthStencilView(DepthStencilView pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil);
	void VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void VSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void PSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShaderResources(unsigned int StartSlot, unsigned int NumViews, ShaderResourceView ppShaderResourceViews);
	void PSSetSamplers(unsigned int StartSlot, unsigned int NumSamplers, ID3D11SamplerState* const* ppSamplers);
	void RSSetState(ID3D11RasterizerState* pRasterizerState);
	void Flush();
	void ClearState();

	void Release();
private:
	ID3D11DeviceContext* m_pImmediateContext = NULL;
};
