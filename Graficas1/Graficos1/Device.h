#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

class Device
{
public:
	Device() = default;
	~Device() = default;

	ID3D11Device*& GetDevicePtr();

	HRESULT CreateRenderTargetView(
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11RenderTargetView** ppRTView);

	HRESULT CreateTexture2D(
		/* [annotation] */
		__in  const D3D11_TEXTURE2D_DESC* pDesc,
		/* [annotation] */
		__in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData,
		/* [annotation] */
		__out  ID3D11Texture2D** ppTexture2D);

	HRESULT CreateDepthStencilView(
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11DepthStencilView** ppDepthStencilView);

	HRESULT CreateVertexShader(
		/* [annotation] */
		__in  const void* pShaderBytecode,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__in_opt  ID3D11ClassLinkage* pClassLinkage,
		/* [annotation] */
		__out_opt  ID3D11VertexShader** ppVertexShader);

	HRESULT CreateInputLayout(
		/* [annotation] */
		__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
		/* [annotation] */
		__in_range(0, D3D11_1_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
		/* [annotation] */
		__in  const void* pShaderBytecodeWithInputSignature,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__out_opt  ID3D11InputLayout** ppInputLayout);

	HRESULT CreatePixelShader(
		/* [annotation] */
		__in  const void* pShaderBytecode,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__in_opt  ID3D11ClassLinkage* pClassLinkage,
		/* [annotation] */
		__out_opt  ID3D11PixelShader** ppPixelShader);

	HRESULT CreateBuffer(
		/* [annotation] */
		__in  const D3D11_BUFFER_DESC* pDesc,
		/* [annotation] */
		__in_opt  const D3D11_SUBRESOURCE_DATA* pInitialData,
		/* [annotation] */
		__out_opt  ID3D11Buffer** ppBuffer);

	HRESULT CreateSamplerState(
		/* [annotation] */
		__in  const D3D11_SAMPLER_DESC* pSamplerDesc,
		/* [annotation] */
		__out_opt  ID3D11SamplerState** ppSamplerState);

	HRESULT CreateRasterizerState(
		/* [annotation] */
		__in  const D3D11_RASTERIZER_DESC* pRasterizerDesc,
		/* [annotation] */
		__out_opt  ID3D11RasterizerState** ppRasterizerState);

	HRESULT CreateShaderResourceView(
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11ShaderResourceView** ppSRView);

	void Release();

private:
	ID3D11Device* m_pd3dDevice = NULL;
};

