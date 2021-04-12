#pragma once

#include <windows.h>
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#elif defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif

#include "RenderManager.h"

#include "Descriptors.h"

#include "Buffer.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"
#include "VertexShader.h"
#include "InputLayout.h"

//#include "Camara.h"
//#include "Mouse.h"
#include "Mesh.h"
#include "OBJInstance.h"
#include "TextureManager.h"


namespace GraphicsModule
{


    class test
	{

		float                               g_ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		RenderManager* g_RenderManager;
#if !defined(OGL)
		HWND                                m_hwnd;
		/**/
#endif
#if defined(DX11)
		DRIVER_TYPE                         g_driverType = DRIVER_TYPE_NULL;
		FEATURE_LEVEL                       g_featureLevel = FEATURE_LEVEL_11_0;


        RenderTargetView                    g_pRenderTargetView;
        DepthStencilView                    g_pDepthStencilView;
        /**/

		SamplerState                        g_pSamplerLinear;
		/**/
		/**/
#endif



    public:
#if defined(DX11)
	    HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
#endif

        HRESULT InitDevice(HWND _hwnd);

        void Resize(unsigned int width, unsigned int height);

        void Update(float dt);
        void Clear();
		void Display();

        void CleanupDevice();

#if defined(DX11)
		inline ID3D11Device*& GetDevice() {
			return g_RenderManager->getDevicePtr();
		}
		inline ID3D11DeviceContext*& GetDeviceContext() { return g_RenderManager->getDeviceContextPtr(); }
		inline IDXGISwapChain*& GetSwapChain() { return g_RenderManager->getSwapChainPtr(); }

#endif
        inline RenderManager* GetRenderManager() { return g_RenderManager; }

        inline void SetDirLight(Vector4 dir) 
		{
			g_RenderManager->UpdateLightDirection(dir);
		}

    private:
#if defined(DX11)
        HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, InputLayout& pInputLayout);
#endif
    };

    extern test& GetTestObj(HWND _hwnd);
}