#define GUI

#include<windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#ifdef GUI
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#endif


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

// -----------------Global var-----------------------------------------------------------------

HINSTANCE                           g_hInst = NULL;
HWND                                g_hwnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;


//ID3D11Device*                       g_pd3dDevice = NULL;
Device*                             g_Device = NULL;

ID3D11DeviceContext*                g_pImmediateContext = NULL;
DeviceContext*                      g_IntermediteContext = NULL;

IDXGISwapChain*                     g_pSwapChain = NULL;
SwapChain*                          g_SwapChain = NULL;


ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11ShaderResourceView*           g_pDepthStencilSRV = NULL;
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
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
ID3D11Buffer*                       g_pVertexBuffer2 = NULL;
ID3D11Buffer*                       g_pIndexBuffer2 = NULL;
ID3D11VertexShader*                 g_pVertexShader2 = NULL;
ID3D11PixelShader*                  g_pPixelShader2 = NULL;
ID3D11RasterizerState*              g_Rasterizer = NULL;
ID3D11RasterizerState*              g_Rasterizer2 = NULL;
ID3D11InputLayout*                  g_pVertexLayout2 = NULL;

D3D11_VIEWPORT                      g_vp;

Camara*                             g_Cameras;
char                                g_CameraCount = 2;
char                                g_ActualCamera = 0;
Mesh*                               g_CubeMesh;
OBJInstance*                        g_OBJInstances;

#ifdef GUI
/**
 * @brief   Forward declare message handler from imgui_impl_win32.cpp
 * @param   #HWND: A handle to the window.
 * @param   #UINT: The message.
 * @param   #WPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @param   #LPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @return  #LRESULT: The return value is the result of the message processing and depends on the message sent..
 * @bug     No know Bugs.
 * @return  #LRESULT: Status code.
 */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
#endif


/**
 * @brief   Message bomb.
 * @bug     No know Bugs.
 * @return  #LRESULT: Status code.
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	#ifdef GUI
	// Handle UI inputs
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam))
		return 1;
	#endif

	switch (message)
	{
	case WM_SIZE:
		//if (g_pd3dDevice != NULL && _wParam != SIZE_MINIMIZED)
	{

		static bool _first = true;
		if (!_first)
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);

			UINT width = rc.right - rc.left;
			UINT height = rc.bottom - rc.top;

			cout << width << ", " << height << endl;

			for (int i = 0; i < g_CameraCount; i++)
			{
				g_Cameras[i].setViewWidth(width);
				g_Cameras[i].setViewHeight(height);
			}

			if (g_pRenderTargetView) g_pRenderTargetView->Release();
			if (g_pDepthStencilView) g_pDepthStencilView->Release();

			//g_pImmediateContext->Flush();
			//g_IntermediteContext->Flush();

			//g_pSwapChain->ResizeBuffers(DXGI_MAX_SWAP_CHAIN_BUFFERS, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, NULL);
			g_SwapChain->ResizeBuffers(DXGI_MAX_SWAP_CHAIN_BUFFERS, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			ID3D11Texture2D* buffer = NULL;
			HRESULT hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

			if (FAILED(hr))
			{
				throw std::exception("Swapchain not created successfully");
			}

			hr = g_Device->CreateRenderTargetView(buffer, NULL, &g_pRenderTargetView);
			buffer->Release();

			if (FAILED(hr))
			{
				throw std::exception("Swapchain not created successfully");
			}

			D3D11_TEXTURE2D_DESC descDepth = {};
			ZeroMemory(&descDepth, sizeof(descDepth));
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

			//hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
			hr = g_Device->CreateTexture2D(&descDepth, nullptr, &buffer);
			if (FAILED(hr))
			{
				throw std::exception("Swapchain not created successfully");
			}

			hr = g_Device->CreateDepthStencilView(buffer, NULL, &g_pDepthStencilView);
			buffer->Release();

			if (FAILED(hr))
			{
				throw std::exception("Swapchain not created successfully");
			}

			g_vp.Width = (FLOAT)width;
			g_vp.Height = (FLOAT)height;
			g_vp.MinDepth = 0.0f;
			g_vp.MaxDepth = 1.0f;
			g_vp.TopLeftX = 0;
			g_vp.TopLeftY = 0;

			/*HRESULT hr = S_OK;

			ID3D11Texture2D* pBackBuffer = NULL;
			//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hr))
				return hr;

			//hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			hr = g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			pBackBuffer->Release();
			if (FAILED(hr))
				return hr;

			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D32_FLOAT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			hr = g_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
			if (FAILED(hr))
				return hr;*/

		}
		_first = !_first;
	}
	return 0;
	break;

	/*case WM_SYSCOMMAND:
		if ((wparam & 0xfff0) == SC_KEYMENU)
		{
			return 0;
		}
		break;*/
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: {
		if (LOWORD(wparam) == 'D')
			g_Cameras[g_ActualCamera].move({ 1, 0, 0 });
		if (LOWORD(wparam) == 'A')
			g_Cameras[g_ActualCamera].move({ -1, 0, 0 });
		if (LOWORD(wparam) == 'W')
			g_Cameras[g_ActualCamera].move({ 0, 1, 0 });
		if (LOWORD(wparam) == 'S')
			g_Cameras[g_ActualCamera].move({ 0, -1, 0 });
		if (LOWORD(wparam) == 'Q')
			g_Cameras[g_ActualCamera].move({ 0, 0, 1 });
		if (LOWORD(wparam) == 'E')
			g_Cameras[g_ActualCamera].move({ 0, 0, -1 });
		if (LOWORD(wparam) == 9)
			g_ActualCamera = (g_ActualCamera + 1) % g_CameraCount;
		//g_Cameras[g_ActualCamera].setIsPerspective(!g_Cameras[g_ActualCamera].getIsPerspective());

		break;
	}
	case WM_MOUSEMOVE:
	{

		break;
	}

	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

/**
 * @brief   Set the style for the main window and init it.
 * @param   #unsigned int: First window width.
 * @param   #unsigned int: First window height.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
HRESULT InitWindow(LONG _width, LONG _height)
{
  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = nullptr;
  wcex.hIcon = nullptr;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = "TutorialWindowClass";
  wcex.hIconSm = nullptr;
  if (!RegisterClassEx(&wcex))
  {
    return E_FAIL;
  }

  // Create window
  RECT rc = { 0, 0, _width, _height };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  g_hwnd = CreateWindow("TutorialWindowClass", "Graficos 1", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, NULL, NULL);
  if (!g_hwnd)
  {
    return E_FAIL;
  }
  ShowWindow(g_hwnd, SW_SHOWNORMAL);

  return S_OK;
}

#ifdef GUI
/**
 * @brief   Init the UI.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
 HRESULT InitImgUI()
 {
   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();

   // Setup Platform/Renderer back ends
   ImGui_ImplWin32_Init(g_hwnd);
   ImGui_ImplDX11_Init(g_Device->GetDevicePtr(), g_IntermediteContext->getDeviceContextPtr());

   return S_OK;
 }
 #endif

HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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

HRESULT InitDevice()
{
  HRESULT hr = S_OK;

  RECT rc;
  GetClientRect(g_hwnd, &rc);
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
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hwnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  g_Device = new Device();
  g_IntermediteContext = new DeviceContext();
  g_SwapChain = new SwapChain();

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
      g_driverType = driverTypes[driverTypeIndex];
      /*hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                          D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );*/
      hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
          D3D11_SDK_VERSION, &sd, &g_SwapChain->GetSwapChainPtr(), &g_Device->GetDevicePtr(), &g_featureLevel, &g_IntermediteContext->getDeviceContextPtr());
      if (SUCCEEDED(hr))
          break;
  }
  if (FAILED(hr))
    return hr;

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = NULL;
  //hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr))
    return hr;

  //hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
  hr = g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr))
    return hr;

  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory(&descDepth, sizeof(descDepth));
  descDepth.Width = width;
  descDepth.Height = height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  //hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
  hr = g_Device->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
  if (FAILED(hr))
    return hr;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = DXGI_FORMAT_D32_FLOAT;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  //hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  hr = g_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if (FAILED(hr))
    return hr;

  // and the resource view for the shader
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1; // same as orig texture
  //hr = g_pd3dDevice->CreateShaderResourceView(g_pDepthStencil, &srvDesc, &g_pDepthStencilSRV);
  hr = g_Device->CreateShaderResourceView(g_pDepthStencil, &srvDesc, &g_pDepthStencilSRV);
  if (FAILED(hr))
    return hr;

  //g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

  // Setup the viewport
  g_vp.Width = (FLOAT)width;
  g_vp.Height = (FLOAT)height;
  g_vp.MinDepth = 0.0f;
  g_vp.MaxDepth = 1.0f;
  g_vp.TopLeftX = 0;
  g_vp.TopLeftY = 0;
  //g_pImmediateContext->RSSetViewports(1, &g_vp);

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
  //hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
  hr = g_Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
  
  if (FAILED(hr))
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
  UINT numElements = ARRAYSIZE(layout);

  // Create the input layout
  //hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
  //  pVSBlob->GetBufferSize(), &g_pVertexLayout);
  hr = g_Device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
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
  //hr = g_pd3dDevice->CreateVertexShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), NULL, &g_pVertexShader2);
  hr = g_Device->CreateVertexShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), NULL, &g_pVertexShader2);
  if (FAILED(hr))
  {
    pVSBlob2->Release();
    return hr;
  }

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout2[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT numElements2 = ARRAYSIZE(layout2);

  // Create the input layout
  //hr = g_pd3dDevice->CreateInputLayout(layout2, numElements2, pVSBlob2->GetBufferPointer(),
  //  pVSBlob2->GetBufferSize(), &g_pVertexLayout2);
  hr = g_Device->CreateInputLayout(layout2, numElements2, pVSBlob2->GetBufferPointer(),
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
  //hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
  hr = g_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
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
  //hr = g_pd3dDevice->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), NULL, &g_pPixelShader2);
  hr = g_Device->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), NULL, &g_pPixelShader2);
  pPSBlob2->Release();
  if (FAILED(hr))
    return hr;

  // Create vertex buffer
  /*SimpleVertex vertices[] =
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
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  //bd.ByteWidth = sizeof(SimpleVertex) * 24;
  bd.ByteWidth = sizeof(Vertex) * g_CubeMesh->getVertexCount();
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));
  //InitData.pSysMem = vertices;
  InitData.pSysMem = g_CubeMesh->getVertex();
  //hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  hr = g_Device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  if (FAILED(hr))
    return hr;

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
  //bd.ByteWidth = sizeof(WORD) * 36;
  bd.ByteWidth = sizeof(unsigned short) * g_CubeMesh->getIndexCount();
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  //InitData.pSysMem = indices;
  InitData.pSysMem = g_CubeMesh->getIndices();
  //hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
  hr = g_Device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
  if (FAILED(hr))
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

  // Create vertex buffer
  /*SimpleVertex vertices2[] =
  {
       { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
  };

  D3D11_BUFFER_DESC bd2;
  ZeroMemory(&bd2, sizeof(bd2));
  bd2.Usage = D3D11_USAGE_DEFAULT;
  bd2.ByteWidth = sizeof(SimpleVertex) * 4;
  bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd2.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData2;
  ZeroMemory(&InitData2, sizeof(InitData2));
  InitData2.pSysMem = vertices2;
  hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_pVertexBuffer2);
  if (FAILED(hr))
    return hr;


  // Create index buffer
  // Create vertex buffer
  WORD indices2[] =
  {
      2,0,1,
      3,0,2
  };

  bd2.Usage = D3D11_USAGE_DEFAULT;
  bd2.ByteWidth = sizeof(WORD) * 6;
  bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bd2.CPUAccessFlags = 0;
  InitData2.pSysMem = indices2;
  hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_pIndexBuffer2);
  if (FAILED(hr))
    return hr;*/



  // Set primitive topology
  //g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Create the constant buffers
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(CBNeverChanges);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  if (FAILED(hr))
    return hr;

  bd.ByteWidth = sizeof(CBChangeOnResize);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
  hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
  if (FAILED(hr))
    return hr;

  bd.ByteWidth = sizeof(CBChangesEveryFrame);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
  hr = g_Device->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
  if (FAILED(hr))
    return hr;

  // Load the Texture
  //hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
  hr = D3DX11CreateShaderResourceViewFromFile(g_Device->GetDevicePtr(), "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
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
  //hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  hr = g_Device->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  
  if (FAILED(hr))
    return hr;

  // Initialize the world matrices
  g_World = XMMatrixIdentity();

  // Initialize the view matrix
  /*XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  g_View = XMMatrixLookAtLH(Eye, At, Up);*/

  RECT rct;
  GetWindowRect(g_hwnd, &rct);

  UINT _width = rct.right - rct.left;
  UINT _height = rct.bottom - rct.top;

  cout << width << ", " << height << endl;

  g_Cameras = new Camara[g_CameraCount];
  g_Cameras[0].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	  _width, _height, 0.01f, 100.0f, true, XM_PIDIV4);
  g_Cameras[1].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	  _width, _height, 0.01f, 100.0f, false, XM_PIDIV4);
  g_View = XMMATRIX(g_Cameras[g_ActualCamera].getViewMatrix());

  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = XMMatrixTranspose(g_View);
  //g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

  // Initialize the projection matrix
  //g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

  /*CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
  g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
  */

  // create rasterizer state
  D3D11_RASTERIZER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.CullMode = D3D11_CULL_BACK;
  desc.FillMode = D3D11_FILL_SOLID;
  //hr = g_pd3dDevice->CreateRasterizerState(&desc, &g_Rasterizer);
  hr = g_Device->CreateRasterizerState(&desc, &g_Rasterizer);
  if (FAILED(hr))
    return hr;

  desc.CullMode = D3D11_CULL_NONE;
  //hr = g_pd3dDevice->CreateRasterizerState(&desc, &g_Rasterizer2);
  hr = g_Device->CreateRasterizerState(&desc, &g_Rasterizer2);
  if (FAILED(hr))
    return hr;

  return S_OK;
}

#ifdef GUI
void UIRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// example window
	if (ImGui::Begin("Another Window", nullptr))
	{
		if (ImGui::Button("Boton", { 100, 40 }))
		{
			
		}
	}
	//ImGui::ShowDemoWindow();
	ImGui::End();

	// render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}
#endif

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

	//
	// Update variables that change once per frame
	//
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.vMeshColor = g_vMeshColor;
	//g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_IntermediteContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);


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



  //UINT stride = sizeof(SimpleVertex);
  //UINT offset = 0;

  //
  // Render the cube
  //
  // Set the input layout
  /*g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
  g_pImmediateContext->RSSetState(g_Rasterizer);
  g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
  g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
  g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
  g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
  g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
  g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
  g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
  g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
  g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
  g_pImmediateContext->DrawIndexed(36, 0, 0);*/
  g_IntermediteContext->IASetInputLayout(g_pVertexLayout);
  g_IntermediteContext->RSSetState(g_Rasterizer);
  g_IntermediteContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
  g_IntermediteContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  g_IntermediteContext->VSSetShader(g_pVertexShader, NULL, 0);
  g_IntermediteContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
  g_IntermediteContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
  g_IntermediteContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
  g_IntermediteContext->PSSetShader(g_pPixelShader, NULL, 0);
  g_IntermediteContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
  g_IntermediteContext->PSSetShaderResources(0, 1, &g_pTextureRV);
  g_IntermediteContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
  //g_IntermediteContext->DrawIndexed(36, 0, 0);


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
  // Render the SAQ
  //
  /*g_pImmediateContext->IASetInputLayout(g_pVertexLayout2);
  g_pImmediateContext->RSSetState(g_Rasterizer2);
  g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer2, &stride, &offset);
  g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer2, DXGI_FORMAT_R16_UINT, 0);
  g_pImmediateContext->VSSetShader(g_pVertexShader2, NULL, 0);
  g_pImmediateContext->PSSetShader(g_pPixelShader2, NULL, 0);*/
  g_IntermediteContext->IASetInputLayout(g_pVertexLayout2);
  g_IntermediteContext->RSSetState(g_Rasterizer2);
  g_IntermediteContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer2, &stride, &offset);
  g_IntermediteContext->IASetIndexBuffer(g_pIndexBuffer2, DXGI_FORMAT_R16_UINT, 0);
  g_IntermediteContext->VSSetShader(g_pVertexShader2, NULL, 0);
  g_IntermediteContext->PSSetShader(g_pPixelShader2, NULL, 0);


  //g_pImmediateContext->DrawIndexed(6, 0, 0);
  //
  // Present our back buffer to our front buffer
  //
  #ifdef GUI
  UIRender();
  #endif
  //g_pSwapChain->Present(0, 0);
  g_SwapChain->Present(0,0);
}

void CleanupDevice()
{
  if (g_pImmediateContext) g_pImmediateContext->ClearState();

  if (g_pSamplerLinear) g_pSamplerLinear->Release();
  if (g_pTextureRV) g_pTextureRV->Release();
  if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
  if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
  if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
  if (g_pVertexBuffer) g_pVertexBuffer->Release();
  if (g_pIndexBuffer) g_pIndexBuffer->Release();
  if (g_pVertexLayout) g_pVertexLayout->Release();
  if (g_pVertexShader) g_pVertexShader->Release();
  if (g_pPixelShader) g_pPixelShader->Release();
  if (g_pDepthStencil) g_pDepthStencil->Release();
  if (g_pDepthStencilView) g_pDepthStencilView->Release();
  if (g_pRenderTargetView) g_pRenderTargetView->Release();
  //if( g_pSwapChain ) g_pSwapChain->Release();
  if (g_SwapChain) g_SwapChain->Release();
  //if( g_pImmediateContext ) g_pImmediateContext->Release();
  if (g_IntermediteContext) g_IntermediteContext->Release();
  //if( g_pd3dDevice ) g_pd3dDevice->Release();
  if (g_Device) g_Device->Release();
}

/**
 * @brief   Entry point.
 * @bug     No know Bugs.
 * @return  #int: Status code.
 */
int main()
{
  // create the window and consolei
  if (FAILED(InitWindow(1280, 720)))
  {
	DestroyWindow(g_hwnd);
	return 0;
  }

  if (FAILED(InitDevice()))
  {
    CleanupDevice();
    return 0;
  }


	#ifdef GUI
  // create UI
  if (FAILED(InitImgUI()))
  {
	  ImGui_ImplDX11_Shutdown();
	  ImGui_ImplWin32_Shutdown();
	  ImGui::DestroyContext();
	  return 0;
  }
  #endif
  // main loop
  MSG msg = { 0 };
  auto start = high_resolution_clock::now();
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
	{
		auto end = high_resolution_clock::now();
		Update(duration<double>(end - start).count());
		start = high_resolution_clock::now();
		Render();
    }
  }
  // clean resources
  #ifdef GUI
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  #endif
  DeleteObject(g_hwnd);
  CleanupDevice();
  return (int)msg.wParam;
}
