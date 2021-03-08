#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#include "RenderManager.h"

#include "Descriptors.h"

#include "Buffer.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"

//#include "Camara.h"
//#include "Mouse.h"
#include "Mesh.h"
#include "OBJInstance.h"
#include "TextureManager.h"

namespace GraphicsModule
{
	struct SimpleVertex
	{
#if defined(DX11)
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
#endif
	};

    class test
    {
#if defined(DX11)
        //D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
        //D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;

        //ID3D11Device*                       g_pd3dDevice = NULL;
        //ID3D11DeviceContext*                g_pImmediateContext = NULL;
        //IDXGISwapChain*                     g_pSwapChain = NULL;

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

        //ID3D11ShaderResourceView*         g_pTextureRV = NULL;
        ID3D11SamplerState*                 g_pSamplerLinear = NULL;
        XMMATRIX                            g_World;

        /*ID3D11Buffer*                     g_pVertexBuffer2 = NULL;
        ID3D11Buffer*                       g_pIndexBuffer2 = NULL;
        /**/

        ID3D11RasterizerState*              g_Rasterizer = NULL;
#endif
		DRIVER_TYPE                         g_driverType = DRIVER_TYPE_NULL;
		FEATURE_LEVEL                       g_featureLevel = FEATURE_LEVEL_11_0;

        float                               g_ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

        //Mesh                                g_Mesh;
        //OBJInstance*                        g_ObjInstances;

        RenderManager*                      g_RenderManager;


        RenderTargetView                    g_pRenderTargetView;
        Texture2D                           g_pDepthStencil;
        DepthStencilView                    g_pDepthStencilView;
        ShaderResourceView                  g_pDepthStencilSRV;
        /**/

		ShaderResourceView                  g_pTextureRV;
        /**/


		Color                               g_vMeshColor;
        /**/


        HWND                                m_hwnd;



        ShaderResourceView                  g_ShaderViews[3];
        RenderTargetView                    g_RenderTargets[3];

    public:
#if defined(DX11)
	    HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
#endif

        HRESULT InitDevice(HWND _hwnd);

        void Resize(unsigned int width, unsigned int height);

        void Update(float dt);
        void Render();

        void CleanupDevice();

#if defined(DX11)
		inline ID3D11Device*& GetDevice() {
			return g_RenderManager->getDevicePtr();
		}
		inline ID3D11DeviceContext*& GetDeviceContext() { return g_RenderManager->getDeviceContextPtr(); }
		inline IDXGISwapChain*& GetSwapChain() { return g_RenderManager->getSwapChainPtr(); }

#endif

		/*inline Camara& getCamera() { return g_Cameras[g_activeCamera]; }

		char getActivaCameraNum() { return g_activeCamera; }
		void setActivaCameraNum(char num) { g_activeCamera = num; }

		char getMaxCameraNum() { return g_CameraCount; }*/

        inline RenderManager* GetRenderManager() { return g_RenderManager; }
    };

    extern test& GetTestObj(HWND _hwnd);
}