#pragma once
#include "Descriptors.h"

#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"

#include "Texture2D.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "Buffer.h"

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

	inline ID3D11Device*& getDevicePtr() { return m_device.GetDevicePtr(); }
	inline ID3D11DeviceContext*& getDeviceContextPtr() { return m_deviceContext.getDeviceContextPtr(); }
	IDXGISwapChain*& GetSwapChainPtr() { return m_swapChain.GetSwapChainPtr(); }

	/*Device functions*/
	HRESULT CreateRenderTargetView( Texture2D& texture, const RENDER_TARGET_VIEW_DESC* desc, RenderTargetView& rtv);
	HRESULT CreateTexture2D( const TEXTURE2D_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Texture2D& ppTexture2D);
	HRESULT CreateDepthStencilView( Texture2D& pResource, const DEPTH_STENCIL_VIEW_DESC* pDesc, DepthStencilView& ppDepthStencilView);
	HRESULT CreateVertexShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, 
							   ID3D11VertexShader** ppVertexShader);
	HRESULT CreateInputLayout( const INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, const void* pShaderBytecodeWithInputSignature, 
							  SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);
	HRESULT CreatePixelShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
							  ID3D11PixelShader** ppPixelShader);
	HRESULT CreateBuffer( const BUFFER_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Buffer& ppBuffer);
	HRESULT CreateSamplerState( const SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
	HRESULT CreateRasterizerState( const RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
	HRESULT CreateShaderResourceView( Texture2D& pResource, const SHADER_RESOURCE_VIEW_DESC* pDesc, ShaderResourceView& ppSRView);

	/*Device context functions*/
	void UpdateSubresource( Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData,
							unsigned int SrcRowPitch, unsigned int SrcDepthPitch );
	void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );
	void OMSetRenderTargets( unsigned int NumViews, RenderTargetView& ppRenderTargetViews, DepthStencilView& pDepthStencilView );
	void RSSetViewports( unsigned int NumViewports, const VIEWPORT* pViewports );
	void IASetInputLayout( ID3D11InputLayout* pInputLayout );
	void IASetVertexBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer& ppVertexBuffers, 
							 const unsigned int* pStrides, const unsigned int* pOffsets);
	void IASetIndexBuffer( Buffer& pIndexBuffer, FORMAT Format, unsigned int Offset);
	void IASetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology);
	void ClearRenderTargetView( RenderTargetView& pRenderTargetView, const float ColorRGBA[4]);
	void ClearDepthStencilView( DepthStencilView& pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil);
	void VSSetShader( ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void VSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShader( ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void PSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShaderResources( unsigned int StartSlot, unsigned int NumViews, ShaderResourceView ppShaderResourceViews);
	void PSSetSamplers( unsigned int StartSlot, unsigned int NumSamplers, ID3D11SamplerState* const* ppSamplers);
	void RSSetState( ID3D11RasterizerState* pRasterizerState);
	void Flush();
	void ClearState();

	/*Swap Chain Functions*/
	HRESULT GetBuffer( unsigned int Buffer, REFIID riid, void** ppSurface);
	HRESULT ResizeBuffers( unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags);
	//HRESULT Resize(unsigned int width, unsigned int height);
	void Present( unsigned int SyncInterval, unsigned int Flags);

	/*Clear*/
	void Release();

private:
	Device m_device;
	DeviceContext m_deviceContext;
	SwapChain m_swapChain;
};

