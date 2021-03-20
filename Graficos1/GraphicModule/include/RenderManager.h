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

#include "VertexShader.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "SamplerState.h"

#include "OBJInstance.h"


namespace GraphicsModule
{
class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

#if defined(DX11)
	inline ID3D11Device*& getDevicePtr() { return m_device.GetDevicePtr(); }
	inline ID3D11DeviceContext*& getDeviceContextPtr() { return m_deviceContext.getDeviceContextPtr(); }
	inline IDXGISwapChain*& getSwapChainPtr() { return m_swapChain.GetSwapChainPtr(); }

	/*Device functions*/
	HRESULT CreateRenderTargetView( Texture2D& texture, const RENDER_TARGET_VIEW_DESC* desc, RenderTargetView& rtv);
	HRESULT CreateTexture2D( const TEXTURE2D_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Texture2D& ppTexture2D);
	HRESULT CreateDepthStencilView( Texture2D& pResource, const DEPTH_STENCIL_VIEW_DESC* pDesc, DepthStencilView& ppDepthStencilView);
	HRESULT CreateVertexShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, VertexShader& ppVertexShader);
	HRESULT CreateInputLayout( const INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, const void* pShaderBytecodeWithInputSignature, 
							  SIZE_T BytecodeLength, InputLayout& ppInputLayout);
	HRESULT CreatePixelShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		PixelShader& ppPixelShader);
	HRESULT CreateBuffer( const BUFFER_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Buffer& ppBuffer);
	HRESULT CreateSamplerState(const SAMPLER_DESC* pSamplerDesc, SamplerState& ppSamplerState);
	HRESULT CreateRasterizerState( const RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
	HRESULT CreateShaderResourceView( Texture2D& pResource, const SHADER_RESOURCE_VIEW_DESC* pDesc, ShaderResourceView& ppSRView);

	/*Device context functions*/
	void UpdateSubresource( Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData,
							unsigned int SrcRowPitch, unsigned int SrcDepthPitch );
	void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );
	void OMSetRenderTargets(unsigned int NumViews, RenderTargetView& ppRenderTargetViews, DepthStencilView& pDepthStencilView);
	void ClearAndSetRenderTargets(unsigned int NumViews, RenderTargetView& ppRenderTargetViews, DepthStencilView& pDepthStencilView, const float ColorRGBA[4]);
	void RSSetViewports( unsigned int NumViewports, const VIEWPORT* pViewports );
	void IASetInputLayout( InputLayout& pInputLayout );
	void IASetVertexBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer& ppVertexBuffers, 
							 const unsigned int* pStrides, const unsigned int* pOffsets);
	void IASetIndexBuffer( Buffer& pIndexBuffer, FORMAT Format, unsigned int Offset);
	void IASetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology);
	void ClearRenderTargetView( RenderTargetView& pRenderTargetView, const float ColorRGBA[4]);
	void ClearDepthStencilView( DepthStencilView& pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil);
	void VSSetShader( VertexShader& pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void VSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShader(PixelShader pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void PSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShaderResources( unsigned int StartSlot, unsigned int NumViews, ShaderResourceView ppShaderResourceViews);
	void PSSetSamplers( unsigned int StartSlot, unsigned int NumSamplers, SamplerState& ppSamplers);
	void RSSetState( ID3D11RasterizerState* pRasterizerState);
	void Flush();
	void ClearState();

	/*Swap Chain Functions*/
	HRESULT GetBuffer( unsigned int Buffer, REFIID riid, void** ppSurface);
	HRESULT ResizeBuffers( unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags);
	//HRESULT Resize(unsigned int width, unsigned int height);
	void Present( unsigned int SyncInterval, unsigned int Flags);

	/*Globals*/
	void DrawObject(OBJInstance* obj);

	HRESULT CreateDeviceAndSwapChain( IDXGIAdapter* pAdapter, DRIVER_TYPE DriverType, HMODULE Software,
		unsigned int Flags, const FEATURE_LEVEL* pFeatureLevels, unsigned int FeatureLevels, unsigned int SDKVersion,
		const SWAP_CHAIN_DESC* pSwapChainDesc, FEATURE_LEVEL* pFeatureLevel);

	HRESULT CreateShaderResourceViewFromFile( LPCSTR pSrcFile, D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
			ShaderResourceView& ppShaderResourceView, HRESULT* pHResult);

	HRESULT CreateShaderAsRenderTargetView(ShaderResourceView& ViewRT, RenderTargetView& RenderTargetView,
		unsigned int width, unsigned int height);

	void UpdateViewMatrix(MATRIX view);
	void UpdateProjectionMatrix(MATRIX projection);
	void UpdateWorld(MATRIX model, Color color);

	Buffer& GetNeverChangesBuffer() { return m_pCBNeverChanges; }
	Buffer& GetChangeOnResizeBuffer() { return m_pCBChangeOnResize; }
	Buffer& GetChangesEveryFrameBuffer() { return m_pCBChangesEveryFrame; }

	/*Clear*/
	void Release();

private:
	Device									m_device;
	DeviceContext							m_deviceContext;
	SwapChain								m_swapChain;


	Buffer									m_pCBNeverChanges;
	Buffer									m_pCBChangeOnResize;
	Buffer									m_pCBChangesEveryFrame;

	UINT									offset = 0;
#endif
};
extern RenderManager* GetManager();

}