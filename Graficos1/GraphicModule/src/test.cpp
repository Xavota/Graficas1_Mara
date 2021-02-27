#include "test.h"


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
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height; 
	//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Format = static_cast<DXGI_FORMAT>(FORMAT_R8G8B8A8_UNORM);
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
      g_driverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
      if (SUCCEEDED(hr))
        break;
    }
    if (FAILED(hr))
      return hr;

    // Create a render target view
	//ID3D11Texture2D* pBackBuffer = NULL;
	Texture2D pBackBuffer;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer.GetTexturePtr());
    if (FAILED(hr))
      return hr;


	//hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer.GetTexturePtr(), NULL, &g_pRenderTargetView.getPtr());
    pBackBuffer.Release();
    if (FAILED(hr))
      return hr;

    // Create depth stencil texture
    //D3D11_TEXTURE2D_DESC descDepth;
    TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1; 
	//descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.Format = FORMAT_R32_TYPELESS;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
	//descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.Usage = USAGE_DEFAULT;
	//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.BindFlags = BIND_DEPTH_STENCIL | BIND_SHADER_RESOURCE;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
	//hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	hr = g_pd3dDevice->CreateTexture2D(reinterpret_cast<D3D11_TEXTURE2D_DESC*>(&descDepth), NULL, &g_pDepthStencil.GetTexturePtr());
    if (FAILED(hr))
      return hr;

    // Create the depth stencil view
	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
	//descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.Format = FORMAT_D32_FLOAT;
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.ViewDimension = DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
	//hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil.GetTexturePtr(), reinterpret_cast<D3D11_DEPTH_STENCIL_VIEW_DESC*>(&descDSV), &g_pDepthStencilView.getStencilViewPtr());
    if (FAILED(hr))
      return hr;
    

    // and the resource view for the shader
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
	//srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Format = FORMAT_R32_FLOAT;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.ViewDimension = SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1; // same as orig texture
	//hr = g_pd3dDevice->CreateShaderResourceView(g_pDepthStencil, &srvDesc, &g_pDepthStencilSRV);
	hr = g_pd3dDevice->CreateShaderResourceView(g_pDepthStencil.GetTexturePtr(), reinterpret_cast<D3D11_SHADER_RESOURCE_VIEW_DESC*>(&srvDesc), &g_pDepthStencilSRV.getPtr());
    if (FAILED(hr))
      return hr;

	//g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView.getPtr(), g_pDepthStencilView.getStencilViewPtr());

    // Setup the viewport
    //D3D11_VIEWPORT vp;
    VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, reinterpret_cast<D3D11_VIEWPORT*>(&vp));

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
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
      pVSBlob->Release();
      return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        //{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 0, static_cast<DXGI_FORMAT>(FORMAT_R32G32B32_FLOAT), 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, static_cast<DXGI_FORMAT>(FORMAT_R32G32_FLOAT), 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
      pVSBlob->GetBufferSize(), &g_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr))
      return hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob2 = NULL;
    hr = CompileShaderFromFile("Limpio.fx", "VS", "vs_4_0", &pVSBlob2);
    if (FAILED(hr))
    {
      MessageBox(NULL,
        "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
      return hr;
    }

    // Create the vertex shader
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), NULL, &g_pVertexShader2);
    if (FAILED(hr))
    {
      pVSBlob2->Release();
      return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout2[] =
    {
        //{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 0, static_cast<DXGI_FORMAT>(FORMAT_R32G32B32_FLOAT), 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, static_cast<DXGI_FORMAT>(FORMAT_R32G32_FLOAT), 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements2 = ARRAYSIZE(layout2);

    // Create the input layout
    hr = g_pd3dDevice->CreateInputLayout(layout2, numElements2, pVSBlob2->GetBufferPointer(),
      pVSBlob2->GetBufferSize(), &g_pVertexLayout2);
    pVSBlob2->Release();
    if (FAILED(hr))
      return hr;

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
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
      return hr;

    // Compile the pixel shader
    ID3DBlob* pPSBlob2 = NULL;
    hr = CompileShaderFromFile("Limpio.fx", "PS", "ps_4_0", &pPSBlob2);
    if (FAILED(hr))
    {
      MessageBox(NULL,
        "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
      return hr;
    }

    // Create the pixel shader
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), NULL, &g_pPixelShader2);
    pPSBlob2->Release();
    if (FAILED(hr))
      return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
    };


    //D3D11_BUFFER_DESC bd;
    BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	bd.Usage = USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.BindFlags = BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd), reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(&InitData), &g_pVertexBuffer.getBufferPtr());
    if (FAILED(hr))
      return hr;

    // Create index buffer
    // Create vertex buffer
    WORD indices[] =
    {
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
    };


    //bd.Usage = D3D11_USAGE_DEFAULT;
    bd.Usage = USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.BindFlags = BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd), reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(&InitData), &g_pIndexBuffer.getBufferPtr());
	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
      return hr;

    // Create vertex buffer
    SimpleVertex vertices2[] =
    {
         { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
    };

	//D3D11_BUFFER_DESC bd2;
	BUFFER_DESC bd2;
    ZeroMemory(&bd2, sizeof(bd2));
	//bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(SimpleVertex) * 4;
	//bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd2.BindFlags = BIND_VERTEX_BUFFER;
    bd2.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData2;
	SUBRESOURCE_DATA InitData2;
    ZeroMemory(&InitData2, sizeof(InitData2));
    InitData2.pSysMem = vertices2;
	//hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_pVertexBuffer2);
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd2), reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(&InitData2), &g_pVertexBuffer2.getBufferPtr());
    if (FAILED(hr))
      return hr;


    // Create index buffer
    // Create vertex buffer
    WORD indices2[] =
    {
        2,0,1,
        3,0,2
    };

	//bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(WORD) * 6;
	//bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.BindFlags = BIND_INDEX_BUFFER;
    bd2.CPUAccessFlags = 0;
	InitData2.pSysMem = indices2;
	//hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_pIndexBuffer2);
    hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd2), reinterpret_cast<D3D11_SUBRESOURCE_DATA*>(&InitData2), &g_pIndexBuffer2.getBufferPtr());
    if (FAILED(hr))
      return hr;



    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    // Create the constant buffers
    //bd.Usage = D3D11_USAGE_DEFAULT;
    bd.Usage = USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.BindFlags = BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd), NULL, &g_pCBNeverChanges.getBufferPtr());
	//hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
    if (FAILED(hr))
      return hr;

    bd.ByteWidth = sizeof(CBChangeOnResize);
	//hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd), NULL, &g_pCBChangeOnResize.getBufferPtr());
    if (FAILED(hr))
      return hr;

    bd.ByteWidth = sizeof(CBChangesEveryFrame);
	//hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
	hr = g_pd3dDevice->CreateBuffer(reinterpret_cast<D3D11_BUFFER_DESC*>(&bd), NULL, &g_pCBChangesEveryFrame.getBufferPtr());
    if (FAILED(hr))
      return hr;

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if (FAILED(hr))
      return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr))
      return hr;

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose(g_View);
	//g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges.getBufferPtr(), 0, NULL, &cbNeverChanges, 0, 0);

    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize.getBufferPtr(), 0, NULL, &cbChangesOnResize, 0, 0);


    // create rasterizer state
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.CullMode = D3D11_CULL_BACK;
    desc.FillMode = D3D11_FILL_SOLID;
    hr = g_pd3dDevice->CreateRasterizerState(&desc, &g_Rasterizer);
    if (FAILED(hr))
      return hr;

    desc.CullMode = D3D11_CULL_NONE;
    hr = g_pd3dDevice->CreateRasterizerState(&desc, &g_Rasterizer2);
    if (FAILED(hr))
      return hr;
    #endif
    return S_OK;
  }

  void test::Render()
  {
#if defined(DX11)
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
    g_World = XMMatrixRotationY(t);

    // Modify the color
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView.getPtr(), ClearColor);

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView.getStencilViewPtr(), static_cast<D3D11_CLEAR_FLAG>(CLEAR_DEPTH), 1.0f, 0);

    //
    // Update variables that change once per frame
    //
    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose(g_World);
    cb.vMeshColor = g_vMeshColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame.getBufferPtr(), 0, NULL, &cb, 0, 0);


    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    //
    // Render the cube
    //
    // Set the input layout
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
    g_pImmediateContext->RSSetState(g_Rasterizer);
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer.getBufferPtr(), &stride, &offset);
	//g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer.getBufferPtr(), static_cast<DXGI_FORMAT>(FORMAT_R16_UINT), 0);
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges.getBufferPtr());
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize.getBufferPtr());
    //g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	//g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame.getBufferPtr());
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	//g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame.getBufferPtr());
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    //
    // Render the SAQ
    //
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout2);
    g_pImmediateContext->RSSetState(g_Rasterizer2);
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer2, &stride, &offset);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer2.getBufferPtr(), &stride, &offset);
	//g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer2, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer2.getBufferPtr(), static_cast<DXGI_FORMAT>(FORMAT_R16_UINT), 0);
    g_pImmediateContext->VSSetShader(g_pVertexShader2, NULL, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader2, NULL, 0);
    //g_pImmediateContext->DrawIndexed(6, 0, 0);
    //
    // Present our back buffer to our front buffer
    //

    //UIRender();
    #endif
  }

  void test::CleanupDevice()
  {
#if defined(DX11)
    if (g_pImmediateContext) g_pImmediateContext->ClearState();

    if (g_pSamplerLinear) g_pSamplerLinear->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    //if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
    //if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
    //if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
	//if (g_pVertexBuffer) g_pVertexBuffer->Release();
	//if (g_pVertexBuffer2) g_pVertexBuffer2->Release();
	//if (g_pIndexBuffer) g_pIndexBuffer->Release();
	//if (g_pIndexBuffer2) g_pIndexBuffer2->Release();
    if (g_pVertexLayout) g_pVertexLayout->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    //if (g_pDepthStencil) g_pDepthStencil->Release();
    //if (g_pDepthStencilView) g_pDepthStencilView->Release();
    //if (g_pRenderTargetView) g_pRenderTargetView.Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    g_pRenderTargetView.Release();
    g_pDepthStencil.Release();
	g_pDepthStencilView.Release();
    g_pDepthStencilSRV.Release();

	g_pVertexBuffer.Release();
	g_pVertexBuffer2.Release();
	g_pIndexBuffer.Release();
	g_pIndexBuffer2.Release();

	g_pCBNeverChanges.Release();
	g_pCBChangeOnResize.Release();
	g_pCBChangesEveryFrame.Release();
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