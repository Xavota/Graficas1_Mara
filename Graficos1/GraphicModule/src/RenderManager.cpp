#include "RenderManager.h"
#include <iostream>
#include <string>

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

void RenderManager::DrawObject(OBJInstance* obj)
{
	/*UINT stride = sizeof(Vertex);
	UINT offset = 0;

	IASetVertexBuffers(0, 1, obj->getMesh()->getVertexBuffer(), &stride, &offset);
	IASetIndexBuffer(obj->getMesh()->getIndexBuffer(), FORMAT_R32_UINT, offset);

	CBChangesEveryFrame cb;

	cb.mWorld =  obj->getModelMatrix();
	Color col = obj->getMesh()->getColor();
	cb.vMeshColor = *reinterpret_cast<XMFLOAT4*>(&col);
	PSSetShaderResources(0, 1, obj->getTexture());
	UpdateSubresource(m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	DrawIndexed(obj->getMesh()->getIndexCount(), 0, 0);*/
}

void RenderManager::UpdateSubresource(Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData, unsigned int SrcRowPitch, unsigned int SrcDepthPitch)
{
	m_deviceContext.UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
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
HRESULT RenderManager::CreateDevices(unsigned int width, unsigned int height, HWND _hwnd, 
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
	sd.OutputWindow = _hwnd;
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
	if (FAILED(hr))
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

HRESULT RenderManager::CompileShaders(const char* vsFileName, LPCSTR vsEntryPoint, LPCSTR vsShaderModel, const char* psFileName, LPCSTR psEntryPoint, LPCSTR psShaderModel)
{
	HRESULT hr = S_OK;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(vsFileName, vsEntryPoint, vsShaderModel, &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, m_vertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}




	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(psFileName, psEntryPoint, psShaderModel, &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, m_pixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;




	CreateInputLayoutDescFromVertexShaderSignature(pVSBlob, m_inputLayout);

	pVSBlob->Release();
	if (FAILED(hr))
		return hr;
	return S_OK;
}
#elif defined(OGL)

void RenderManager::CompileShaders(const char* vsFileName, const char* psFileName)
{
	m_shader.Init(vsFileName, psFileName);
}

void RenderManager::ShaderSetBool(const string name, bool value)
{
	m_shader.SetBool(name, value);
}

void RenderManager::ShaderSetInt(const string name, int value)
{
	m_shader.SetInt(name, value);
}

void RenderManager::ShaderSetFloat(const string name, int value)
{
	m_shader.SetFloat(name, value);
}

void RenderManager::ShaderSetUint(const string name, unsigned int value)
{
	m_shader.SetUint(name, value);
}

void RenderManager::ShaderSetBool2(const string name, bool value1, bool value2)
{
	m_shader.SetBool2(name, value1, value2);
}

void RenderManager::ShaderSetInt2(const string name, int value1, int value2)
{
	m_shader.SetInt2(name, value1, value2);
}

void RenderManager::ShaderSetFloat2(const string name, float value1, float value2)
{
	m_shader.SetFloat2(name, value1, value2);
}

void RenderManager::ShaderSetUint2(const string name, unsigned int value1, unsigned int value2)
{
	m_shader.SetUint2(name, value1, value2);
}

void RenderManager::ShaderSetBool3(const string name, bool value1, bool value2, bool value3)
{
	m_shader.SetBool3(name, value1, value2, value3);
}

void RenderManager::ShaderSetInt3(const string name, int value1, int value2, int value3)
{
	m_shader.SetInt3(name, value1, value2, value3);
}

void RenderManager::ShaderSetFloat3(const string name, float value1, float value2, float value3)
{
	m_shader.SetFloat3(name, value1, value2, value3);
}

void RenderManager::ShaderSetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
{
	m_shader.SetUint3(name, value1, value2, value3);
}

void RenderManager::ShaderSetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
{
	m_shader.SetBool4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetInt4(const string name, int value1, int value2, int value3, int value4)
{
	m_shader.SetInt4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetFloat4(const string name, float value1, float value2, float value3, float value4)
{
	m_shader.SetFloat4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
{
	m_shader.SetUint4(name, value1, value2, value3, value4);
}

void RenderManager::ShaderSetMat4(const string name, glm::mat4 value)
{
	m_shader.SetMat4(name, value);
}

#endif

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
	XMMATRIX g_View = *reinterpret_cast<XMMATRIX*>(&view);
	ViewMat cbNeverChanges;
	cbNeverChanges.view = XMMatrixTranspose(g_View);
	UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
#elif defined(OGL)
	m_shader.SetMat4("view", glm::mat4(view._11, view._12, view._13, view._14, 
											 view._21, view._22, view._23, view._24, 
											 view._31, view._32, view._33, view._34, 
											 view._41, view._42, view._43, view._44));
#endif           
}

void RenderManager::UpdateProjectionMatrix(MATRIX projection)
{
#if defined(DX11)
	XMMATRIX g_Projection = *reinterpret_cast<XMMATRIX*>(&projection);
	ProjectionMat cbChangesOnResize;
	cbChangesOnResize.projection = XMMatrixTranspose(g_Projection);
	UpdateSubresource(m_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
#elif defined(OGL)
	m_shader.SetMat4("projection", glm::mat4(projection._11, projection._12, projection._13, projection._14, 
											 projection._21, projection._22, projection._23, projection._24, 
											 projection._31, projection._32, projection._33, projection._34, 
											 projection._41, projection._42, projection._43, projection._44));
#endif          
}

void RenderManager::UpdateModelMatrix(MATRIX model)
{
#if defined(DX11)
	XMMATRIX g_Model = *reinterpret_cast<XMMATRIX*>(&model);
	ModelMat modl;
	modl.model = XMMatrixTranspose(g_Model);
	UpdateSubresource(m_pCBChangesEveryFrame, 0, NULL, &modl, 0, 0);
#elif defined(OGL)
	m_shader.SetMat4("model", glm::mat4(model._11, model._12, model._13, model._14,
										model._21, model._22, model._23, model._24,
										model._31, model._32, model._33, model._34,
										model._41, model._42, model._43, model._44));
#endif   
}

void RenderManager::UpdateLightDirection(Vector4 dir)
{
#if defined(DX11)
	DirLight dirDesc;
	dirDesc.DIR = dir;
	UpdateSubresource(m_DirLightBuffer, 0, NULL, &dirDesc, 0, 0);
#elif defined(OGL)
	m_shader.SetFloat4("lightDir", dir.x, dir.y, dir.z, dir.w);
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
	m_DirLightBuffer.Release();

	m_vertexShader.Release();
	m_pixelShader.Release();
	m_inputLayout.Release();
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