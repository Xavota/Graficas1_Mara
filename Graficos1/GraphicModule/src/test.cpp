#include "test.h"

#include <iostream>

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
		hr = g_RenderManager->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
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
		hr = g_RenderManager->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;
			



		// Define the input layout
		INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, FORMAT_R32G32_FLOAT, 0, 12, INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = g_RenderManager->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &g_pVertexLayout);/**/

		pVSBlob->Release();
		if (FAILED(hr))
		  return hr;




		// Create vertex buffer
		g_Mesh.setVertex(new Vertex[24]{
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

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * 24;
		bd.BindFlags = BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = g_Mesh.getVertex();
		hr = g_RenderManager->CreateBuffer(&bd, &InitData, g_pVertexBuffer);/**/
		if (FAILED(hr))
		    return hr;




		// Create index buffer
		g_Mesh.setIndices(new unsigned short[36]{
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

		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * g_Mesh.getIndexCount();
		bd.BindFlags = BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = g_Mesh.getIndices();
		hr = g_RenderManager->CreateBuffer(&bd, &InitData, g_pIndexBuffer);/**/
		if (FAILED(hr))
		    return hr;




		g_ObjInstances = new OBJInstance[4];
		g_ObjInstances[0].setMesh(&g_Mesh);
		g_ObjInstances[0].setPosition({ 0,0,4 });
		g_ObjInstances[0].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[0].setSize({ 2,2,2 });

		g_ObjInstances[1].setMesh(&g_Mesh);
		g_ObjInstances[1].setPosition({ -5,0,3.5 });
		g_ObjInstances[1].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[1].setSize({ 1,1,1 });

		g_ObjInstances[2].setMesh(&g_Mesh);
		g_ObjInstances[2].setPosition({ 5,0,3.5 });
		g_ObjInstances[2].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[2].setSize({ 1,1,1 });

		g_ObjInstances[3].setMesh(&g_Mesh);
		g_ObjInstances[3].setPosition({ 0,5,3.75f });
		g_ObjInstances[3].setRotation({ 0,0,3.14159265 });
		g_ObjInstances[3].setSize({ 1.5,1.5,1.5 });




		// Set primitive topology
		g_RenderManager->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);





		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBNeverChanges);
		bd.BindFlags = BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_pCBNeverChanges);
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(CBChangeOnResize);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_pCBChangeOnResize);
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(CBChangesEveryFrame);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_pCBChangesEveryFrame);/**/
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
		hr = g_RenderManager->CreateSamplerState(&sampDesc, &g_pSamplerLinear);/**/
		if (FAILED(hr))
		  return hr;




		// Initialize the world matrices
		g_World = XMMatrixIdentity();

		g_Cameras = new Camara[2];
		g_Cameras[0].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
			width, height, 0.01f, 100.0f, true, XM_PIDIV4);
		g_Cameras[1].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
			width, height, 0.01f, 100.0f, false, XM_PIDIV4);

		// Initialize the view matrix
		g_View = XMMATRIX(g_Cameras[g_activeCamera].getViewMatrix());

		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = XMMatrixTranspose(g_View);
		g_RenderManager->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

		// Initialize the projection matrix
		g_Projection = XMMATRIX(g_Cameras[g_activeCamera].getProjectionMatrix());

		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
		g_RenderManager->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);




		RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.CullMode = CULL_BACK;
		desc.FillMode = FILL_SOLID;
		hr = g_RenderManager->CreateRasterizerState(&desc, &g_Rasterizer);
		if (FAILED(hr))
			return hr;




		/*Load textures*/

		hr = g_RenderManager->CreateShaderResourceViewFromFile("cheems2.dds", NULL, NULL, g_pTextureRV, NULL);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[0].setTexture(g_pTextureRV);


		Texture2D Texture;

		g_RenderManager->CreateShaderAndRenderTargetView(Texture, g_pViewRT2, g_pRenderTargetView2, width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[1].setTexture(g_pViewRT2);
		Texture.Release();


		g_RenderManager->CreateShaderAndRenderTargetView(Texture, g_pViewRT3, g_pRenderTargetView3, width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[2].setTexture(g_pViewRT3);
		Texture.Release();


		g_RenderManager->CreateShaderAndRenderTargetView(Texture, g_pViewRT4, g_pRenderTargetView4, width, height);
		if (FAILED(hr))
			return hr;

		g_ObjInstances[3].setTexture(g_pViewRT4);
		Texture.Release();

#endif
		return S_OK;
	}

	void test::Update(float dt)
	{
#if defined(DX11)

		/*Update the mouse position*/
		LPPOINT p = new POINT;
		GetCursorPos(p);
		Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
		delete p;


		/*Update the active camera*/
		g_Cameras[g_activeCamera].Update();


		/*Set the new view and projection matrices*/
		g_View = XMMATRIX(g_Cameras[g_activeCamera].getViewMatrix());
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = XMMatrixTranspose(g_View);
		g_RenderManager->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

		g_Projection = XMMATRIX(g_Cameras[g_activeCamera].getProjectionMatrix());
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
		g_RenderManager->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);


		// Update our time
		static float t = 0.0f;
		if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			  t += (float)XM_PI * 0.0125f;
		}
		else
		{
			  static DWORD dwTimeStart = 0;
			  DWORD dwTimeCur = GetTickCount();
			  if (dwTimeStart == 0)
				  dwTimeStart = dwTimeCur;
			  t = (dwTimeCur - dwTimeStart) / 1000.0f;
		}

		// Rotate cube around the origin
		//g_World = XMMatrixRotationY(t);


		// Modify the color
		/*(sinf(t * 1.0f) + 1.0f) * 0.5f;
		  (cosf(t * 3.0f) + 1.0f) * 0.5f;
		  (sinf(t * 5.0f) + 1.0f) * 0.5f;/**/
		g_vMeshColor.r = .7;
		g_vMeshColor.g = .7;
		g_vMeshColor.b = .7;/**/

		g_ObjInstances[0].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[1].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[2].getMesh()->setColor(g_vMeshColor);
		g_ObjInstances[3].getMesh()->setColor(g_vMeshColor);

#endif
	}

	void test::Render()
	{
#if defined(DX11)

		UINT offset = 0;

		// Set the input layout
		g_RenderManager->IASetInputLayout(g_pVertexLayout);
		g_RenderManager->RSSetState(g_Rasterizer);

		g_RenderManager->VSSetShader(g_pVertexShader, NULL, 0);
		g_RenderManager->VSSetConstantBuffers(0, 1, g_pCBNeverChanges);
		g_RenderManager->VSSetConstantBuffers(1, 1, g_pCBChangeOnResize);
		g_RenderManager->VSSetConstantBuffers(2, 1, g_pCBChangesEveryFrame);
		g_RenderManager->PSSetShader(g_pPixelShader, NULL, 0);
		g_RenderManager->PSSetConstantBuffers(2, 1, g_pCBChangesEveryFrame);
		g_RenderManager->PSSetSamplers(0, 1, &g_pSamplerLinear);


		// Render the cubes

		/*Primer Cubo*/
		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView2, g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0], g_pCBChangesEveryFrame, &offset);


		/*Segundo Cubo*/
		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView3, g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[1], g_pCBChangesEveryFrame, &offset);


		/*Tercer Cubo*/
		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView4, g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[1], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[2], g_pCBChangesEveryFrame, &offset);


		/*Cuarto Cubo*/
		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView, g_ClearColor);
		g_RenderManager->DrawObject(&g_ObjInstances[0], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[1], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[2], g_pCBChangesEveryFrame, &offset);
		g_RenderManager->DrawObject(&g_ObjInstances[3], g_pCBChangesEveryFrame, &offset);

#endif
	}

  void test::CleanupDevice()
  {
#if defined(DX11)

	  if (g_pSamplerLinear) g_pSamplerLinear->Release();

	  if (g_pVertexLayout) g_pVertexLayout->Release();
	  if (g_pVertexShader) g_pVertexShader->Release();
	  if (g_pPixelShader) g_pPixelShader->Release();

	  g_pTextureRV.Release();

	  g_pRenderTargetView.Release();
	  g_pDepthStencil.Release();
	  g_pDepthStencilView.Release();
	  g_pDepthStencilSRV.Release();

	  g_pVertexBuffer.Release();
	  g_pIndexBuffer.Release();

	  g_pCBNeverChanges.Release();
	  g_pCBChangeOnResize.Release();
	  g_pCBChangesEveryFrame.Release();


	  g_pViewRT2.Release();
	  g_pRenderTargetView2.Release();

	  g_pViewRT3.Release();
	  g_pRenderTargetView3.Release();
;
	  g_pViewRT4.Release();
	  g_pRenderTargetView4.Release();

	  g_RenderManager->Release();

#endif

	  delete[] g_ObjInstances;
  }

  void test::Resize(unsigned int width, unsigned int height)
  {
	  for (int i = 0; i < g_CameraCount; i++)
	  {
		  g_Cameras[i].setViewWidth(width);
		  g_Cameras[i].setViewHeight(height);
	  }

#if defined(DX11)

	  g_pRenderTargetView.Release();
	  g_pRenderTargetView2.Release();
	  g_pRenderTargetView3.Release();
	  g_pRenderTargetView4.Release();
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


	  g_pViewRT2.Release();

	  g_RenderManager->CreateShaderAndRenderTargetView(buffer, g_pViewRT2, g_pRenderTargetView2, width, height);
	  buffer.Release();
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  g_ObjInstances[1].setTexture(g_pViewRT2);


	  g_pViewRT3.Release();

	  g_RenderManager->CreateShaderAndRenderTargetView(buffer, g_pViewRT3, g_pRenderTargetView3, width, height);
	  buffer.Release();
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  g_ObjInstances[2].setTexture(g_pViewRT3);


	  g_pViewRT4.Release();

	  g_RenderManager->CreateShaderAndRenderTargetView(buffer, g_pViewRT4, g_pRenderTargetView4, width, height);
	  buffer.Release();
	  if (FAILED(hr))
	  {
		  throw std::runtime_error("Swapchain not created successfully");
	  }

	  g_ObjInstances[3].setTexture(g_pViewRT4);




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