#include "test.h"

#include <iostream>
#include <vector>

namespace GraphicsModule
{
#if defined(DX11)
    HRESULT test::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
#endif

	HRESULT test::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, InputLayout& pInputLayout)
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
		HRESULT hr = g_RenderManager->CreateInputLayout(reinterpret_cast<const INPUT_ELEMENT_DESC*>(&inputLayoutDesc[0]), inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
		return hr;
	}

	HRESULT test::InitDevice(HWND _hwnd)
	{
#if defined(DX11)

		m_hwnd = _hwnd;

		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;

#ifdef _DEBUG

		createDeviceFlags |= CREATE_DEVICE_DEBUG;

#endif

		g_RenderManager = GetManager();

		



		DRIVER_TYPE driverTypes[] =
		{
			DRIVER_TYPE_HARDWARE,
			DRIVER_TYPE_WARP,
			DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		FEATURE_LEVEL featureLevels[] =
		{
			FEATURE_LEVEL_11_0,
			FEATURE_LEVEL_10_1,
			FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

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

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			g_driverType = driverTypes[driverTypeIndex];
			hr = g_RenderManager->CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			  D3D11_SDK_VERSION, &sd, &g_featureLevel);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
		  return hr;




		Texture2D pBackBuffer;
		hr = g_RenderManager->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer.GetTexturePtr());
		if (FAILED(hr))
			return hr;

		hr = g_RenderManager->CreateRenderTargetView(pBackBuffer, NULL, g_pRenderTargetView);
		pBackBuffer.Release();
		if (FAILED(hr))
		  return hr;




		TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = USAGE_DEFAULT;
		descDepth.BindFlags = BIND_DEPTH_STENCIL | BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = g_RenderManager->CreateTexture2D(&descDepth, NULL, g_pDepthStencil);/**/
		if (FAILED(hr))
		  return hr;




		DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = FORMAT_D32_FLOAT;
		descDSV.ViewDimension = DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = g_RenderManager->CreateDepthStencilView(g_pDepthStencil, &descDSV, g_pDepthStencilView);/**/
		if (FAILED(hr))
		  return hr;




		SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1; // same as orig texture
		hr = g_RenderManager->CreateShaderResourceView(g_pDepthStencil, &srvDesc, g_pDepthStencilSRV);/**/
		if (FAILED(hr))
		  return hr;

		g_RenderManager->OMSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView);




		// Setup the viewportç
		VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_RenderManager->RSSetViewports(1, &vp);




		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
		  MessageBox(NULL,
		    "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		  return hr;
		}

		// Create the vertex shader
		hr = g_RenderManager->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, g_pVertexShader);
		if (FAILED(hr))
		{
		  pVSBlob->Release();
		  return hr;
		}




		// Compile the pixel shader
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		// Create the pixel shader
		hr = g_RenderManager->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, g_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;
			



		CreateInputLayoutDescFromVertexShaderSignature(pVSBlob, g_RenderManager->getDevicePtr(), g_pVertexLayout);

		pVSBlob->Release();
		if (FAILED(hr))
		  return hr;




		// Create vertex buffer
		/*hr = g_Mesh.setVertex(new Vertex[24]{
			{ Vector3{-1.0f, 1.0f, -1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{1.0f, 1.0f, -1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{1.0f, 1.0f, 1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{-1.0f, 1.0f, 1.0f}, Vector2{0.0f, 1.0f} },

			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, -1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, 1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{-1.0f, -1.0f, 1.0f}, Vector2{0.0f, 1.0f} },

			{ Vector3{-1.0f, -1.0f, 1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{-1.0f, 1.0f, -1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{-1.0f, 1.0f, 1.0f}, Vector2{0.0f, 1.0f} },

			{ Vector3{1.0f, -1.0f, 1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, -1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{1.0f, 1.0f, -1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{1.0f, 1.0f, 1.0f}, Vector2{0.0f, 1.0f} },

			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, -1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{1.0f, 1.0f, -1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{-1.0f, 1.0f, -1.0f}, Vector2{0.0f, 1.0f} },

			{ Vector3{-1.0f, -1.0f, 1.0f}, Vector2{0.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, 1.0f}, Vector2{1.0f, 0.0f} },
			{ Vector3{1.0f, 1.0f, 1.0f}, Vector2{1.0f, 1.0f} },
			{ Vector3{-1.0f, 1.0f, 1.0f}, Vector2{0.0f, 1.0f} } }, 24);

		if (FAILED(hr))
		    return hr;




		// Create index buffer
		hr = g_Mesh.setIndices(new unsigned short[36]{
		    3,1,0,
		    2,1,3,

		    6,4,5,
		    7,4,6,

		    11,9,8,
		    10,9,11,

		    14,12,13,
		    15,12,14,

		    19,17,16,
		    18,17,19,

		    22,20,21,
		    23,20,22
		    }, 36
		);

		if (FAILED(hr))
		    return hr;*/




		/*g_ObjInstances = new OBJInstance[4];

		g_ObjInstances[0].setMesh(&g_Mesh);
		g_ObjInstances[0].setPosition({ 0,0,2 });
		g_ObjInstances[0].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[0].setSize({ 1,1,1 });

		g_ObjInstances[1].setMesh(&g_Mesh);
		g_ObjInstances[1].setPosition({ -3,0,2 });
		g_ObjInstances[1].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[1].setSize({ 1,1,1 });

		g_ObjInstances[2].setMesh(&g_Mesh);
		g_ObjInstances[2].setPosition({ 3,0,2 });
		g_ObjInstances[2].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[2].setSize({ 1,1,1 });

		g_ObjInstances[3].setMesh(&g_Mesh);
		g_ObjInstances[3].setPosition({ 0,3,2 });
		g_ObjInstances[3].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[3].setSize({ 1,1,1 });*/




		// Set primitive topology
		g_RenderManager->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);





		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBNeverChanges);
		bd.BindFlags = BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetNeverChangesBuffer());
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(CBChangeOnResize);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetChangeOnResizeBuffer());
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(CBChangesEveryFrame);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetChangesEveryFrameBuffer());/**/
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(DirLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_DirLightBuffer);/**/
		if (FAILED(hr))
			return hr;




		SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = FLOAT32_MAX;
		hr = g_RenderManager->CreateSamplerState(&sampDesc, g_pSamplerLinear);/**/
		if (FAILED(hr))
		  return hr;




		// Initialize the world matrices
		g_World = XMMatrixIdentity();




		RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.CullMode = CULL_BACK;
		desc.FillMode = FILL_SOLID;
		hr = g_RenderManager->CreateRasterizerState(&desc, &g_Rasterizer);
		if (FAILED(hr))
			return hr;




		/*Load textures*/

		//hr = g_RenderManager->CreateShaderResourceViewFromFile("cheems2.dds", NULL, NULL, g_pTextureRV, NULL);
		//TextureManager::CreateTextureFromFile("cheems2.dds", "Cheems");
		//if (FAILED(hr))
		//	return hr;

		//g_ObjInstances[0].setTexture(g_pTextureRV);
		//g_ObjInstances[0].setTexture(TextureManager::GetTexture("Cheems"));


		/*g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[0], g_RenderTargets[0], width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[1].setTexture(g_ShaderViews[0]);


		g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[1], g_RenderTargets[1], width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[2].setTexture(g_ShaderViews[1]);


		g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[2], g_RenderTargets[2], width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[3].setTexture(g_ShaderViews[2]);*/

#endif
		return S_OK;
	}

	void test::Update(float dt)
	{
#if defined(DX11)
		static float dtt = 0;
		dtt += dt;


		// Modify the color
		/*(sinf(t * 1.0f) + 1.0f) * 0.5f;
		  (cosf(t * 3.0f) + 1.0f) * 0.5f;
		  (sinf(t * 5.0f) + 1.0f) * 0.5f;/**/
		g_vMeshColor;
		g_vMeshColor.r = .7;
		g_vMeshColor.g = .7;
		g_vMeshColor.b = .7;/**/

		/*g_ObjInstances[0].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[1].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[2].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[3].getMesh()->setColor(g_vMeshColor);/**/

		static Vector rotation_c{ 0,0,0 };
		rotation_c.setVector(0, dtt, 3.14159265);

		/*g_ObjInstances[0].setRotation(rotation_c);
		g_ObjInstances[1].setRotation(rotation_c);
		g_ObjInstances[2].setRotation(rotation_c);
		g_ObjInstances[3].setRotation(rotation_c);/**/


		g_RenderManager->UpdateSubresource(g_DirLightBuffer, 0, NULL, &g_dirLightDesc, 0, 0);
#endif
	}

	void test::Render()
	{
#if defined(DX11)

		// Set the input layout
		g_RenderManager->IASetInputLayout(g_pVertexLayout);
		g_RenderManager->RSSetState(g_Rasterizer);

		g_RenderManager->VSSetShader(g_pVertexShader, NULL, 0);
		g_RenderManager->VSSetConstantBuffers(0, 1, g_RenderManager->GetNeverChangesBuffer());
		g_RenderManager->VSSetConstantBuffers(1, 1, g_RenderManager->GetChangeOnResizeBuffer());
		g_RenderManager->VSSetConstantBuffers(2, 1, g_RenderManager->GetChangesEveryFrameBuffer());
		g_RenderManager->VSSetConstantBuffers(3, 1, g_DirLightBuffer);

		g_RenderManager->PSSetShader(g_pPixelShader, NULL, 0);
		g_RenderManager->PSSetConstantBuffers(2, 1, g_RenderManager->GetChangesEveryFrameBuffer());
		g_RenderManager->PSSetSamplers(0, 1, g_pSamplerLinear);


		// Render the cubes

		/*Primer Cubo*/
		/*g_RenderManager->ClearAndSetRenderTargets(1, g_RenderTargets[0], g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0]);


		/*Segundo Cubo*/
		/*g_RenderManager->ClearAndSetRenderTargets(1, g_RenderTargets[1], g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0]);
		g_RenderManager->DrawObject(&g_ObjInstances[1]);


		/*Tercer Cubo*/
		/*g_RenderManager->ClearAndSetRenderTargets(1, g_RenderTargets[2], g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0]);
		g_RenderManager->DrawObject(&g_ObjInstances[1]);
		g_RenderManager->DrawObject(&g_ObjInstances[2]);


		/*Cuarto Cubo*/
		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView, g_ClearColor);
		/*g_RenderManager->DrawObject(&g_ObjInstances[0]);
		g_RenderManager->DrawObject(&g_ObjInstances[1]);
		g_RenderManager->DrawObject(&g_ObjInstances[2]);
		g_RenderManager->DrawObject(&g_ObjInstances[3]);*/

#endif
	}

  void test::CleanupDevice()
  {
#if defined(DX11)

	  g_pSamplerLinear.Release();

	  g_pVertexLayout.Release();
	  g_pVertexShader.Release();
	  g_pPixelShader.Release();

	  g_RenderManager->ClearState();

	  g_pTextureRV.Release();

	  g_pRenderTargetView.Release();
	  g_pDepthStencil.Release();
	  g_pDepthStencilView.Release();
	  g_pDepthStencilSRV.Release();

	  /*g_pVertexBuffer.Release();
	  g_pIndexBuffer.Release();/**/

	  /*g_pCBNeverChanges.Release();
	  g_pCBChangeOnResize.Release();
	  g_pCBChangesEveryFrame.Release();*/

	  g_DirLightBuffer.Release();


	  g_ShaderViews[0].Release();
	  g_ShaderViews[1].Release();
	  g_ShaderViews[2].Release();

	  g_RenderTargets[0].Release();
	  g_RenderTargets[1].Release();
	  g_RenderTargets[2].Release();


	  g_RenderManager->Release();

#endif

	  //delete[] g_ObjInstances;
	  delete g_RenderManager;
  }

  void test::Resize(unsigned int width, unsigned int height)
  {

#if defined(DX11)

	  g_pRenderTargetView.Release();
	  g_RenderTargets[0].Release();
	  g_RenderTargets[1].Release();
	  g_RenderTargets[2].Release();
	  g_pDepthStencilView.Release();


	  g_RenderManager->ResizeBuffers(MAX_SWAP_CHAIN_BUFFERS, width, height, FORMAT_R8G8B8A8_UNORM, 0);


	  Texture2D buffer;
	  HRESULT hr = g_RenderManager->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer.GetTexturePtr());
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }


	  hr = g_RenderManager->CreateRenderTargetView(buffer, NULL, g_pRenderTargetView);

	  buffer.Release();
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }


	  g_ShaderViews[0].Release();

	  g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[0], g_RenderTargets[0], width, height);
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  //g_ObjInstances[1].setTexture(g_ShaderViews[0]);


	  g_ShaderViews[1].Release();

	  g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[1], g_RenderTargets[1], width, height);
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  //g_ObjInstances[2].setTexture(g_ShaderViews[1]);


	  g_ShaderViews[2].Release();

	  g_RenderManager->CreateShaderAsRenderTargetView(g_ShaderViews[2], g_RenderTargets[2], width, height);
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  //g_ObjInstances[3].setTexture(g_ShaderViews[2]);


	

	  TEXTURE2D_DESC descDepth = {};
	  ZeroMemory(&descDepth, sizeof(descDepth));
	  descDepth.Width = width;
	  descDepth.Height = height;
	  descDepth.MipLevels = 1;
	  descDepth.ArraySize = 1;
	  descDepth.Format = FORMAT_D24_UNORM_S8_UINT;
	  descDepth.SampleDesc.Count = 1;
	  descDepth.SampleDesc.Quality = 0;
	  descDepth.Usage = USAGE_DEFAULT;
	  descDepth.BindFlags = BIND_DEPTH_STENCIL;
	  descDepth.CPUAccessFlags = 0;
	  descDepth.MiscFlags = 0;

	  hr = g_RenderManager->CreateTexture2D(&descDepth, nullptr, buffer);
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  hr = g_RenderManager->CreateDepthStencilView(buffer, NULL, g_pDepthStencilView);
	  buffer.Release();

	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  VIEWPORT vp;
	  vp.Width = (FLOAT)width;
	  vp.Height = (FLOAT)height;
	  vp.MinDepth = 0.0f;
	  vp.MaxDepth = 1.0f;
	  vp.TopLeftX = 0;
	  vp.TopLeftY = 0;
	  g_RenderManager->RSSetViewports(1, &vp);

#endif
  }

  extern test& GetTestObj(HWND _hwnd)
  {
	  static test* pTest = nullptr;
	  if (nullptr == pTest)
	  {
		  pTest = new test();
		  pTest->InitDevice(_hwnd);
	  }
	  return *pTest;
  }
}