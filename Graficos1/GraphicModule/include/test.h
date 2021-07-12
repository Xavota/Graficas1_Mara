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
		string m_Technique = "Deferred";

		RenderManager* g_RenderManager;

        RenderTargetView                    g_pRenderTargetView;
        DepthStencilView                    g_pDepthStencilView;

#if defined(DX11)
		DRIVER_TYPE                         g_driverType = DRIVER_TYPE_NULL;
		FEATURE_LEVEL                       g_featureLevel = FEATURE_LEVEL_11_0;

		SamplerState                        g_pSamplerLinear;
		/**/
#endif



    public:
        HRESULT InitDevice();

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

		inline void SetViewPosAndDir(Vector4 pos, Vector4 dir)
		{
#if defined(DX11)
			/*g_RenderManager->UpdateSubresource(g_RenderManager->GetViewPositionBuffer(), 0, NULL, &pos, 0, 0);
			g_RenderManager->PSSetConstantBuffers(3, 1, g_RenderManager->GetViewPositionBuffer());/**/
			//ShaderChange
			//GraphicsModule::GetManager()->getShader().SetBuffer(3, g_RenderManager->GetViewPositionBuffer(), &pos);
			GraphicsModule::GetManager()->getShader("Deferred").SetEffectValue("ViewPosition", &pos);
			GraphicsModule::GetManager()->getShader("Forward").SetEffectValue("ViewPosition", &pos);
#elif defined(OGL)
			GraphicsModule::GetManager()->getShader("Deferred").SetEffectValue("ViewPosition", &pos);
			GraphicsModule::GetManager()->getShader("Forward").SetEffectValue("ViewPosition", &pos);
#endif
		}

		inline void SetTechnique(string tech)
		{
			m_Technique = tech;
		}

		inline string GetTechnique()
		{
			return m_Technique;
		}

    private:
#if defined(DX11)
        HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, InputLayout& pInputLayout);
#endif
    };

    extern test& GetTestObj();
}