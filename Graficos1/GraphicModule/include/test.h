#pragma once

#include <windows.h>
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

#include "Descriptors.h"

#include "Buffer.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"


namespace GraphicsModule
{

	struct VIEWPORT
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

  struct SimpleVertex
  {
    #if defined(DX11)
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
    #endif
  };

  struct CBNeverChanges
  {
    #if defined(DX11)
    XMMATRIX mView;
    #endif
  };

  struct CBChangeOnResize
  {
    #if defined(DX11)
    XMMATRIX mProjection;
    #endif
  };

  struct CBChangesEveryFrame
  {
    #if defined(DX11)
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
    #endif
  };

  class test
  {
  private:
#if defined(DX11)
    D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    ID3D11Device*                       g_pd3dDevice = NULL;
    ID3D11DeviceContext*                g_pImmediateContext = NULL;
    IDXGISwapChain*                     g_pSwapChain = NULL;

    /*ID3D11RenderTargetView*           g_pRenderTargetView = NULL;
	ID3D11Texture2D*                    g_pDepthStencil = NULL;
	ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
	ID3D11ShaderResourceView*           g_pDepthStencilSRV = NULL;
    /**/

    ID3D11VertexShader*                 g_pVertexShader = NULL;
    ID3D11PixelShader*                  g_pPixelShader = NULL;
    ID3D11InputLayout*                  g_pVertexLayout = NULL;

    /*ID3D11Buffer*                     g_pVertexBuffer = NULL;
    ID3D11Buffer*                       g_pIndexBuffer = NULL;
    ID3D11Buffer*                       g_pCBNeverChanges = NULL;
    ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
    ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
    /**/

    ID3D11ShaderResourceView*           g_pTextureRV = NULL;
    ID3D11SamplerState*                 g_pSamplerLinear = NULL;
    XMMATRIX                            g_World;
    XMMATRIX                            g_View;
    XMMATRIX                            g_Projection;
    XMFLOAT4                            g_vMeshColor;
    
    /*ID3D11Buffer*                     g_pVertexBuffer2 = NULL;
    ID3D11Buffer*                       g_pIndexBuffer2 = NULL;
    /**/

    ID3D11VertexShader*                 g_pVertexShader2 = NULL;
    ID3D11PixelShader*                  g_pPixelShader2 = NULL;
    ID3D11RasterizerState*              g_Rasterizer = NULL;
    ID3D11RasterizerState*              g_Rasterizer2 = NULL;
    ID3D11InputLayout*                  g_pVertexLayout2 = NULL;
#endif

	RenderTargetView g_pRenderTargetView;
	Texture2D g_pDepthStencil;
	DepthStencilView g_pDepthStencilView;
	ShaderResourceView g_pDepthStencilSRV;
    /**/

	Buffer g_pVertexBuffer;
	Buffer g_pIndexBuffer;
	Buffer g_pCBNeverChanges;
	Buffer g_pCBChangeOnResize;
	Buffer g_pCBChangesEveryFrame;
	/**/
    
	Buffer g_pVertexBuffer2;
	Buffer g_pIndexBuffer2;
	/**/

	HWND m_hwnd;
  public:
    #if defined(DX11)
    HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
    #endif
    HRESULT InitDevice(HWND _hwnd);

    void Render();

    void CleanupDevice();

  public:
	  inline ID3D11Device*& GetDevice() { return g_pd3dDevice; }
	  inline ID3D11DeviceContext*& GetDeviceContext() { return g_pImmediateContext; }
	  inline IDXGISwapChain*& GetSwapChain() { return g_pSwapChain; }
  };

  extern test& GetTestObj(HWND _hwnd);

}