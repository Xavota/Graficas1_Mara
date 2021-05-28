#include "RenderManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

HRESULT RenderManager::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, VertexShader& ppVertexShader)
{
	return m_device.CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT RenderManager::CreateInputLayout(const INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, InputLayout& ppInputLayout)
{
	return m_device.CreateInputLayout(reinterpret_cast<const D3D11_INPUT_ELEMENT_DESC*>(pInputElementDescs), NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

HRESULT RenderManager::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, PixelShader& ppPixelShader)
{
	return m_device.CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT RenderManager::CreateBuffer(const BUFFER_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Buffer& ppBuffer)
{
	return m_device.CreateBuffer(reinterpret_cast<const D3D11_BUFFER_DESC*>(pDesc), reinterpret_cast<const D3D11_SUBRESOURCE_DATA*>(pInitialData), &ppBuffer.getBufferPtr());
}

HRESULT RenderManager::CreateSamplerState(const SAMPLER_DESC* pSamplerDesc, SamplerState& ppSamplerState)
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

void RenderManager::UpdateSubresource(Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData, unsigned int SrcRowPitch, unsigned int SrcDepthPitch)
{
	m_deviceContext.UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void RenderManager::UpdateTexture2D(Texture2D& image, const void* data, unsigned int rowSize)
{
	m_deviceContext.UpdateTexture2D(image, data, rowSize);
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

void RenderManager::IASetInputLayout(InputLayout& pInputLayout)
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

void RenderManager::VSSetShader(VertexShader& pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
{
	m_deviceContext.VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void RenderManager::VSSetConstantBuffers(unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers)
{
	m_deviceContext.VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void RenderManager::PSSetShader(PixelShader pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances)
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

void RenderManager::PSSetSamplers(unsigned int StartSlot, unsigned int NumSamplers, SamplerState& ppSamplers)
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
HRESULT RenderManager::CreateDevices(unsigned int width, unsigned int height, 
									 const DRIVER_TYPE* driverTypes, unsigned int numDriverTypes, unsigned int Flags, 
									 const FEATURE_LEVEL* pFeatureLevels, unsigned int numFeatureLevels, FEATURE_LEVEL* pFeatureLevel)
{
	HRESULT hr = S_OK;

	SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		DRIVER_TYPE g_driverType = driverTypes[driverTypeIndex];
		hr = CreateDeviceAndSwapChain(NULL, g_driverType, NULL, Flags, pFeatureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, pFeatureLevel);
		if (SUCCEEDED(hr))
			break;
	}
	return hr;
}

HRESULT RenderManager::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFileA(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT RenderManager::CompileShaderFromString(const char* source, unsigned int bytesCount, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompile(source, bytesCount, nullptr, nullptr, nullptr,
		szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT RenderManager::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, InputLayout& pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = CreateInputLayout(reinterpret_cast<const INPUT_ELEMENT_DESC*>(&inputLayoutDesc[0]), inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}

void RenderManager::SetBuffer(int slot, Buffer buff, void* data)
{
	//m_shader.SetBuffer(slot, buff, data);
	m_effect.SetBuffer(slot, buff, data);
}

#elif defined(OGL)

void RenderManager::ShaderSetBool(const string name, bool value)
{
	m_effect.SetBool(name, value);
}

void RenderManager::ShaderSetInt(const string name, int value)
{
	m_effect.SetInt(name, value);
}

void RenderManager::ShaderSetFloat(const string name, float value)
{
	m_effect.SetFloat(name, value);
}

void RenderManager::ShaderSetUint(const string name, unsigned int value)
{
	m_effect.SetUint(name, value);
}

void RenderManager::ShaderSetBool2(const string name, bool value1, bool value2)
{
	m_effect.SetBool2(name, value1, value2);
}

void RenderManager::ShaderSetInt2(const string name, int value1, int value2)
{
	m_effect.SetInt2(name, value1, value2);
}

void RenderManager::ShaderSetFloat2(const string name, float value1, float value2)
{
	m_effect.SetFloat2(name, value1, value2);
}

void RenderManager::ShaderSetUint2(const string name, unsigned int value1, unsigned int value2)
{
	m_effect.SetUint2(name, value1, value2);
}

void RenderManager::ShaderSetBool3(const string name, bool value1, bool value2, bool value3)
{
	m_effect.SetBool3(name, value1, value2, value3);
}

void RenderManager::ShaderSetInt3(const string name, int value1, int value2, int value3)
{
	m_effect.SetInt3(name, value1, value2, value3);
}

void RenderManager::ShaderSetFloat3(const string name, float value1, float value2, float value3)
{
	m_effect.SetFloat3(name, value1, value2, value3);
}

void RenderManager::ShaderSetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
{
	m_effect.SetUint3(name, value1, value2, value3);
}

void RenderManager::ShaderSetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
{
	m_effect.SetBool4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetInt4(const string name, int value1, int value2, int value3, int value4)
{
	m_effect.SetInt4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetFloat4(const string name, float value1, float value2, float value3, float value4)
{
	m_effect.SetFloat4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
{
	m_effect.SetUint4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetMat4(const string name, glm::mat4 value)
{
	m_effect.SetMat4(name, value);
}

#endif

HRESULT RenderManager::CompileShaders(const char* vsFileName, const char* psFileName)
{
	m_effect.CompileShader(vsFileName, psFileName);
	SetShaderFlags(eNORMAL_TECHNIQUES::NONE, eSPECULAR_TECHNIQUES::NONE, 0);
	return S_OK;
}

void RenderManager::SetShaderFlags(eNORMAL_TECHNIQUES nor, eSPECULAR_TECHNIQUES spec, unsigned int texFlags)
{
	m_effect.SetShaderFlags(nor, spec, texFlags);
}

void RenderManager::setViewport(unsigned int width, unsigned int height)
{
#if defined(DX11)
	VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	RSSetViewports(1, &vp);
#elif defined(OGL)
	glViewport(0, 0, width, height);
#endif
}

void RenderManager::UpdateViewMatrix(MATRIX view)
{
#if defined(DX11)
	ViewMat cbNeverChanges;
	cbNeverChanges.view = view.TransposeMatrix();
	m_effect.SetBuffer(0, m_pCBNeverChanges, &cbNeverChanges);
#elif defined(OGL)
	m_effect.SetMat4("view", glm::mat4(view._11, view._12, view._13, view._14,
											 view._21, view._22, view._23, view._24, 
											 view._31, view._32, view._33, view._34, 
											 view._41, view._42, view._43, view._44));
#endif           
}

void RenderManager::UpdateProjectionMatrix(MATRIX projection)
{
#if defined(DX11)
	ProjectionMat cbChangesOnResize;
	cbChangesOnResize.projection = projection.TransposeMatrix();
	m_effect.SetBuffer(1, m_pCBChangeOnResize, &cbChangesOnResize);
#elif defined(OGL)
	m_effect.SetMat4("projection", glm::mat4(projection._11, projection._12, projection._13, projection._14,
											 projection._21, projection._22, projection._23, projection._24, 
											 projection._31, projection._32, projection._33, projection._34, 
											 projection._41, projection._42, projection._43, projection._44));
#endif          
}

void RenderManager::UpdateModelMatrix(MATRIX model)
{
#if defined(DX11)
	ModelMat modl;
	modl.model = model;
	m_effect.SetBuffer(2, m_pCBChangesEveryFrame, &model);
#elif defined(OGL)
	m_effect.SetMat4("model", glm::mat4(model._11, model._12, model._13, model._14,
										model._21, model._22, model._23, model._24,
										model._31, model._32, model._33, model._34,
										model._41, model._42, model._43, model._44));
#endif   
}

void RenderManager::UpdateDirectionalLight(DirectionalLight dirDesc)
{
#if defined(DX11)
	m_effect.SetBuffer(5, m_DirectionalLightBuffer, &dirDesc);
#elif defined(OGL)
	m_effect.SetFloat4("dirLight.lightDir", dirDesc.lightDir.x, dirDesc.lightDir.y, dirDesc.lightDir.z, dirDesc.lightDir.w);
	m_effect.SetFloat4("dirLight.ambient", dirDesc.ambient.x, dirDesc.ambient.y, dirDesc.ambient.z, dirDesc.ambient.w);
	m_effect.SetFloat4("dirLight.diffuse", dirDesc.diffuse.x, dirDesc.diffuse.y, dirDesc.diffuse.z, dirDesc.diffuse.w);
	m_effect.SetFloat4("dirLight.specular", dirDesc.specular.x, dirDesc.specular.y, dirDesc.specular.z, dirDesc.specular.w);
#endif
}

void RenderManager::UpdatePointLight(PointLight pointDesc)
{
#if defined(DX11)
	m_effect.SetBuffer(6, m_PointLightBuffer, &pointDesc);
#elif defined(OGL)
	m_effect.SetFloat4("pointLight.lightPos", pointDesc.lightPos.x, pointDesc.lightPos.y, pointDesc.lightPos.z, 0);
	m_effect.SetFloat4("pointLight.diffuse", pointDesc.diffuse.x, pointDesc.diffuse.y, pointDesc.diffuse.z, 1);
	m_effect.SetFloat4("pointLight.specular", pointDesc.specular.x, pointDesc.specular.y, pointDesc.specular.z, 1);
	m_effect.SetFloat("pointLight.blurDistance", pointDesc.blurDistance);
#endif
}

void RenderManager::UpdateSpotLight(SpotLight spotDesc)
{
#if defined(DX11)
	spotDesc.cutOff = cos((spotDesc.cutOff * 2) * 3.1415 / 180 - 45);
	spotDesc.outerCutOff = cos((spotDesc.outerCutOff * 2) * 3.1415 / 180 - 45);
	m_effect.SetBuffer(7, m_SpotLightBuffer, &spotDesc);
#elif defined(OGL)
	m_effect.SetFloat4("spotLight.lightPos", spotDesc.lightPos.x, spotDesc.lightPos.y, spotDesc.lightPos.z, 0);
	m_effect.SetFloat4("spotLight.lightDir", spotDesc.lightDir.x, spotDesc.lightDir.y, spotDesc.lightDir.z, 1);
	m_effect.SetFloat("spotLight.cutOff", glm::cos(glm::radians(spotDesc.cutOff)));
	m_effect.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotDesc.outerCutOff)));
	m_effect.SetFloat4("spotLight.diffuse", spotDesc.diffuse.x, spotDesc.diffuse.y, spotDesc.diffuse.z, 1);
	m_effect.SetFloat4("spotLight.specular", spotDesc.specular.x, spotDesc.specular.y, spotDesc.specular.z, 1);
	m_effect.SetFloat("spotLight.blurDistance", spotDesc.blurDistance);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Release*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderManager::Release()
{
#if defined(DX11)
	m_device.Release();
	m_deviceContext.Release();
	m_swapChain.Release();

	m_pCBNeverChanges.Release();
	m_pCBChangeOnResize.Release();
	m_pCBChangesEveryFrame.Release();
	m_DirectionalLightBuffer.Release();
	m_PointLightBuffer.Release();
	m_SpotLightBuffer.Release();
#endif           
}

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