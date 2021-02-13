//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource.h"
#include "Camara.h"
#include "Mesh.h"
#include "OBJInstance.h"
#include "Mouse.h"

#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"

#include <chrono>
#include <iostream>
using namespace std;
using namespace chrono;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;

//ID3D11Device*                       g_pd3dDevice = NULL;
Device*                             g_Device = NULL;

//ID3D11DeviceContext*                g_pImmediateContext = NULL;
DeviceContext*                      g_IntermediteContext = NULL;

//IDXGISwapChain*                     g_pSwapChain = NULL;
SwapChain*                          g_SwapChain = NULL;

ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor( 0.7f, 0.7f, 0.7f, 1.0f );
Camara*                             g_Cameras;
char                                g_CameraCount = 2;
char                                g_ActualCamera = 0;
Mesh*                               g_CubeMesh;
OBJInstance*                        g_OBJInstances;
D3D11_VIEWPORT                      g_vp;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Update(float dt);
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    auto start = high_resolution_clock::now();
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            auto end = high_resolution_clock::now();
            Update(duration<double>(end - start).count());
            start = high_resolution_clock::now();
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
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
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
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
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    g_Device = new Device();
    g_IntermediteContext = new DeviceContext();
    g_SwapChain = new SwapChain();

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        /*hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );*/
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
		                                	D3D11_SDK_VERSION, &sd, &g_SwapChain->GetSwapChainPtr(), &g_Device->GetDevicePtr(), &g_featureLevel, &g_IntermediteContext->getDeviceContextPtr());
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if( FAILED( hr ) )
        return hr;

    //hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    hr = g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    //hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
    hr = g_Device->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);

    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    //hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    hr = g_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);

    if( FAILED( hr ) )
        return hr;


    // Setup the viewport
    
    g_vp.Width = (FLOAT)width;
    g_vp.Height = (FLOAT)height;
    g_vp.MinDepth = 0.0f;
    g_vp.MaxDepth = 1.0f;
    g_vp.TopLeftX = 0;
	g_vp.TopLeftY = 0;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the vertex shader
    //hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
    hr = g_Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);

    if( FAILED( hr ) )
    {    
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
    //hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
    //                                      pVSBlob->GetBufferSize(), &g_pVertexLayout );
	hr = g_Device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                     pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if( FAILED( hr ) )
        return hr;

	// Set the input layout

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the pixel shader
    //hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
    hr = g_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);

    pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    /*SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
    };*/

    g_CubeMesh = new Mesh();

    g_CubeMesh->setVertex(new Vertex[24]{
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


    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    //bd.ByteWidth = sizeof( SimpleVertex ) * 24;
    bd.ByteWidth = sizeof(Vertex) * g_CubeMesh->getVertexCount();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    //InitData.pSysMem = vertices;
    InitData.pSysMem = g_CubeMesh->getVertex();
    //hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    hr = g_Device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer

    // Create index buffer
    // Create vertex buffer
    /*WORD indices[] =
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
    };*/

    g_CubeMesh->setIndices(new unsigned short[36]{
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

    bd.Usage = D3D11_USAGE_DEFAULT;
    //bd.ByteWidth = sizeof( WORD ) * 36;
    bd.ByteWidth = sizeof(unsigned short) * g_CubeMesh->getIndexCount();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
	//InitData.pSysMem = indices;
	InitData.pSysMem = g_CubeMesh->getIndices();
    //hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
    hr = g_Device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if( FAILED( hr ) )
        return hr;

	g_OBJInstances = new OBJInstance[4];
	g_OBJInstances[0].setMesh(g_CubeMesh);
	g_OBJInstances[0].setPosition({ 0,0,2 });
	g_OBJInstances[1].setMesh(g_CubeMesh);
	g_OBJInstances[1].setPosition({ -3,0,2 });
	g_OBJInstances[2].setMesh(g_CubeMesh);
	g_OBJInstances[2].setPosition({ 3,0,2 });
	g_OBJInstances[3].setMesh(g_CubeMesh);
	g_OBJInstances[3].setPosition({ 0,3,2 });


    // Create the constant buffers
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    //hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBNeverChanges );
    hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangeOnResize);
    //hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
    hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    //hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );
    hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
    if( FAILED( hr ) )
        return hr;

    // Load the Texture
    //hr = D3DX11CreateShaderResourceViewFromFile( g_pd3dDevice, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );
    hr = D3DX11CreateShaderResourceViewFromFile(g_Device->GetDevicePtr(), L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    //hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
    hr = g_Device->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

    if( FAILED( hr ) )
        return hr;

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    //g_View = XMMatrixLookAtLH( Eye, At, Up );
    /*g_Camera.setEyePos({ 0.0f, 3.0f, -6.0f });
    g_Camera.setLookAt({ 0.0f, 1.0f, 0.0f });
    g_Camera.setWorldUpVector({ 0.0f, 1.0f, 0.0f });*/

    g_Cameras = new Camara[g_CameraCount];
    g_Cameras[0].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, true, XM_PIDIV4);
	g_Cameras[1].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, false, XM_PIDIV4);
    g_View = XMMATRIX(g_Cameras[g_ActualCamera].getViewMatrix());


    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose( g_View );

    // Initialize the projection matrix
    //g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f );
    //g_Projection = XMMATRIX(Camara::getPerspectiveMatrix(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f));

    //g_Projection = XMMatrixOrthographicLH(width, height, 0.01f, 100.0f);
    //g_Projection = XMMATRIX(Camara::getOrtograficMatrix(width, height, 0.01f, 100.0f));

	g_Projection = XMMATRIX(g_Cameras[g_ActualCamera].getProjectionMatrix());

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    //if( g_pImmediateContext ) g_pImmediateContext->ClearState();
    if ( g_IntermediteContext ) g_IntermediteContext->ClearState();

    if( g_pSamplerLinear ) g_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_pCBNeverChanges ) g_pCBNeverChanges->Release();
    if( g_pCBChangeOnResize ) g_pCBChangeOnResize->Release();
    if( g_pCBChangesEveryFrame ) g_pCBChangesEveryFrame->Release();
    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    //if( g_pSwapChain ) g_pSwapChain->Release();
    if ( g_SwapChain ) g_SwapChain->Release();
    //if( g_pImmediateContext ) g_pImmediateContext->Release();
    if ( g_IntermediteContext ) g_IntermediteContext->Release();
    //if( g_pd3dDevice ) g_pd3dDevice->Release();
    if ( g_Device ) g_Device->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        case WM_KEYDOWN: {
            if (LOWORD(wParam) == 'D')
                g_Cameras[g_ActualCamera].move({ 1, 0, 0 });
			if (LOWORD(wParam) == 'A')
                g_Cameras[g_ActualCamera].move({ -1, 0, 0 });
			if (LOWORD(wParam) == 'W')
                g_Cameras[g_ActualCamera].move({ 0, 1, 0 });
			if (LOWORD(wParam) == 'S')
                g_Cameras[g_ActualCamera].move({ 0, -1, 0 });
			if (LOWORD(wParam) == 'Q')
                g_Cameras[g_ActualCamera].move({ 0, 0, 1 });
			if (LOWORD(wParam) == 'E')
                g_Cameras[g_ActualCamera].move({ 0, 0, -1 });
            if (LOWORD(wParam) == 9)
                g_ActualCamera = (g_ActualCamera + 1) % g_CameraCount;
                //g_Cameras[g_ActualCamera].setIsPerspective(!g_Cameras[g_ActualCamera].getIsPerspective());

            break; 
        }
        case WM_MOUSEMOVE:
        {
            
            break; 
        }

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

void Update(float dt) 
{



	g_View = XMMATRIX(g_Cameras[g_ActualCamera].getViewMatrix());
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_View);
	//g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
    g_IntermediteContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	g_Projection = XMMATRIX(g_Cameras[g_ActualCamera].getProjectionMatrix());
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
	//g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
    g_IntermediteContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

    // Update our time
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * dt;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // Modify the color
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

	// Rotate cube around the origin
	g_World = XMMatrixRotationY(t); 

    LPPOINT p = new POINT;
	GetCursorPos(p);
	Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
    delete p;

    g_Cameras[g_ActualCamera].Update();
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{

	/*g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pImmediateContext->RSSetViewports(1, &g_vp);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);*/
    g_IntermediteContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
    g_IntermediteContext->RSSetViewports(1, &g_vp);
    g_IntermediteContext->IASetInputLayout(g_pVertexLayout);

	// Set index buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	/*g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);*/
    g_IntermediteContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    g_IntermediteContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_IntermediteContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    //g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
    g_IntermediteContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    g_IntermediteContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


    //
    // Render the cube
    //
    /*g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
    g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
    g_pImmediateContext->VSSetConstantBuffers( 1, 1, &g_pCBChangeOnResize );
    g_pImmediateContext->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
    g_pImmediateContext->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
    g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );*/
	g_IntermediteContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_IntermediteContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
	g_IntermediteContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	g_IntermediteContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_IntermediteContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_IntermediteContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_IntermediteContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	g_IntermediteContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

	XMMATRIX trans;
    Vector pos1;

	//
	// Update variables that change once per frame
	//
	CBChangesEveryFrame cb;

	pos1 = g_OBJInstances[0].getPosition();
	cb.mWorld = XMMatrixMultiplyTranspose(g_World, XMMatrixTranslation(pos1.x(), pos1.y(), pos1.z()));
	cb.vMeshColor = g_vMeshColor;
	/*g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_pImmediateContext->DrawIndexed(g_OBJInstances[0].getMesh()->getIndexCount(), 0, 0);*/
    g_IntermediteContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_IntermediteContext->DrawIndexed(g_OBJInstances[0].getMesh()->getIndexCount(), 0, 0);

	pos1 = g_OBJInstances[1].getPosition();
	cb.mWorld = XMMatrixMultiplyTranspose(g_World, XMMatrixTranslation(pos1.x(), pos1.y(), pos1.z()));
	cb.vMeshColor = g_vMeshColor;
    /*g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_pImmediateContext->DrawIndexed(g_OBJInstances[0].getMesh()->getIndexCount(), 0, 0);*/
    g_IntermediteContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_IntermediteContext->DrawIndexed(g_OBJInstances[1].getMesh()->getIndexCount(), 0, 0);

	pos1 = g_OBJInstances[2].getPosition();
	cb.mWorld = XMMatrixMultiplyTranspose(g_World, XMMatrixTranslation(pos1.x(), pos1.y(), pos1.z()));
	cb.vMeshColor = g_vMeshColor;
    /*g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_pImmediateContext->DrawIndexed(g_OBJInstances[0].getMesh()->getIndexCount(), 0, 0);*/
    g_IntermediteContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_IntermediteContext->DrawIndexed(g_OBJInstances[2].getMesh()->getIndexCount(), 0, 0);

	pos1 = g_OBJInstances[3].getPosition();
	cb.mWorld = XMMatrixMultiplyTranspose(g_World, XMMatrixTranslation(pos1.x(), pos1.y(), pos1.z()));
	cb.vMeshColor = g_vMeshColor;
    /*g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_pImmediateContext->DrawIndexed(g_OBJInstances[0].getMesh()->getIndexCount(), 0, 0);*/
    g_IntermediteContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_IntermediteContext->DrawIndexed(g_OBJInstances[3].getMesh()->getIndexCount(), 0, 0);

    //
    // Present our back buffer to our front buffer
    //
    //g_pSwapChain->Present( 0, 0 );
    g_SwapChain->Present(0, 0);
}
