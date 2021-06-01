#include "test.h"

#include <iostream>
#include <vector>

namespace GraphicsModule
{          

	HRESULT test::InitDevice()
	{
		g_RenderManager = GetManager();

#if defined(DX11)

		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(g_RenderManager->GetWindow(), &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;

#ifdef _DEBUG

		createDeviceFlags |= CREATE_DEVICE_DEBUG;

#endif

		/*Create device, device context and swap chain*/
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

		if (FAILED(g_RenderManager->CreateDevices(width, height, driverTypes, numDriverTypes, createDeviceFlags, featureLevels, numFeatureLevels, &g_featureLevel)))
			return E_FAIL;



		/*Create the back buffer*/
		Texture2D pBackBuffer;
		hr = g_RenderManager->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer.GetTexturePtr());
		if (FAILED(hr))
			return hr;

		hr = g_RenderManager->CreateRenderTargetView(pBackBuffer, NULL, g_pRenderTargetView);
		pBackBuffer.Release();
		if (FAILED(hr))
		  return hr;



		/*Create depth stencil*/
		Texture2D g_pDepthStencil;

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
		{
			g_pDepthStencil.Release();
			return hr;
		}

		DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = FORMAT_D32_FLOAT;
		descDSV.ViewDimension = DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = g_RenderManager->CreateDepthStencilView(g_pDepthStencil, &descDSV, g_pDepthStencilView);/**/
		g_pDepthStencil.Release();
		if (FAILED(hr))
			return hr;

		//g_RenderManager->OMSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView);



		/*Compile vertex and pixel shaders*/
		//g_RenderManager->CompileShaders("Tutorial07.fx", "main", "vs_4_0", "Tutorial07.fx", "PS", "ps_4_0");
		//g_RenderManager->CompileShaders("miVertex.fx", "main", "vs_4_0", "miPixel.fx", "main", "ps_4_0");




		/*Create buffers that pass data to the shaders*/
		/*BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ProjectionMat);
		bd.BindFlags = BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetNeverChangesBuffer());
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(ViewMat);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetChangeOnResizeBuffer());
		if (FAILED(hr))
		  return hr;

		bd.ByteWidth = sizeof(ModelMat);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetChangesEveryFrameBuffer());/**/
		/*if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(Vector4);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetViewPositionBuffer());/**/
		/*if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(Material);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetMaterialShininessBuffer());/**/
		/*if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(DirectionalLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetDirectionalLightBuffer());/**/
		/*if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(PointLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetPointLightBuffer());/**/
		/*if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(SpotLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetSpotLightBuffer());/**/
		/*if (FAILED(hr))
			return hr;



		/*Create sampler state*/
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
		g_RenderManager->PSSetSamplers(0, 1, g_pSamplerLinear);















		Texture2D g_pTextRT2;
		TEXTURE2D_DESC descTextRT;
		ZeroMemory(&descTextRT, sizeof(descTextRT));
		descTextRT.Width = 1264;
		descTextRT.Height = 681;
		descTextRT.MipLevels = 1;
		descTextRT.ArraySize = 1;
		descTextRT.Format = FORMAT_R8G8B8A8_UNORM;
		descTextRT.SampleDesc.Count = 1;
		descTextRT.SampleDesc.Quality = 0;
		descTextRT.Usage = USAGE_DEFAULT;
		descTextRT.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
		descTextRT.CPUAccessFlags = 0;
		descTextRT.MiscFlags = 0;
		if (FAILED(g_RenderManager->CreateTexture2D(&descTextRT, NULL, g_pTextRT2)))
			return E_FAIL;

		// create the rt Shader resource view
		SHADER_RESOURCE_VIEW_DESC descViewRT;
		ZeroMemory(&descViewRT, sizeof(descViewRT));
		descViewRT.Format = FORMAT_R8G8B8A8_UNORM;
		descViewRT.ViewDimension = SRV_DIMENSION_TEXTURE2D;
		descViewRT.Texture2D.MostDetailedMip = 0;
		descViewRT.Texture2D.MipLevels = 1;
		if (FAILED(g_RenderManager->CreateShaderResourceView(g_pTextRT2, &descViewRT, srv)))
			return E_FAIL;

		// Create the render target view
		if (FAILED(g_RenderManager->CreateRenderTargetView(g_pTextRT2, NULL, RTV2)))
			return E_FAIL;

		g_pTextRT2.Release();
#elif defined(OGL)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//g_RenderManager->CompileShaders("miVertex.fx", "miPixel.fx");
#endif

		g_RenderManager->getShader().CreatePass("GBuffer", "GBufferVertex.fx", "GBufferPixel.fx");
		g_RenderManager->getShader().CreatePass("Lights", "LightVertex.fx", "LightPixel.fx");
#if defined(DX11)
		g_RenderManager->getShader().AddEffectTrackValue("ViewMatrix", 0, sizeof(ViewMat));
		g_RenderManager->getShader().AddEffectTrackValue("ProjectionMatrix", 1, sizeof(ProjectionMat));
		g_RenderManager->getShader().AddEffectTrackValue("ModelMatrix", 2, sizeof(ModelMat));
		g_RenderManager->getShader().AddEffectTrackValue("ViewPosition", 3, sizeof(Vector4));
		g_RenderManager->getShader().AddEffectTrackValue("Material", 4, sizeof(Material));
		g_RenderManager->getShader().AddEffectTrackValue("DirectionalLight", 5, sizeof(DirectionalLight));
		g_RenderManager->getShader().AddEffectTrackValue("PointLight", 6, sizeof(PointLight));
		g_RenderManager->getShader().AddEffectTrackValue("SpotLight", 7, sizeof(SpotLight));

		g_RenderManager->getShader().AddPassInputTexture("GBuffer", "diffuse");
		g_RenderManager->getShader().AddPassInputTexture("GBuffer", "normal");
		g_RenderManager->getShader().AddPassInputTexture("GBuffer", "specular");
		g_RenderManager->getShader().AddPassOutputTexture("GBuffer", "Position");
		g_RenderManager->getShader().AddPassOutputTexture("GBuffer", "Color");
		g_RenderManager->getShader().AddPassOutputTexture("GBuffer", "Normal");
		g_RenderManager->getShader().AddPassOutputTexture("GBuffer", "Specular");

		g_RenderManager->getShader().AddPassInputTexture("Lights", "position");
		g_RenderManager->getShader().AddPassInputTexture("Lights", "diffuse");
		g_RenderManager->getShader().AddPassInputTexture("Lights", "normal");
		g_RenderManager->getShader().AddPassInputTexture("Lights", "specular");
		g_RenderManager->getShader().AddPassOutputTexture("Lights", "Final");

		g_RenderManager->getShader().UniteInputOutputTextures("GBuffer", "Position", "Lights", "position");
		g_RenderManager->getShader().UniteInputOutputTextures("GBuffer", "Color", "Lights", "diffuse");
		g_RenderManager->getShader().UniteInputOutputTextures("GBuffer", "Normal", "Lights", "normal");
		g_RenderManager->getShader().UniteInputOutputTextures("GBuffer", "Specular", "Lights", "specular");
#elif defined(OGL)
#endif
		return S_OK;
	}

	void test::Update(float dt)
	{
		/*Update viewport*/
		int width = 0, height = 0;
#if defined(DX11)
		RECT rc;
		GetClientRect(g_RenderManager->GetWindow(), &rc);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
#elif defined(OGL)
		glfwGetFramebufferSize(g_RenderManager->GetWindow(), &width, &height);
#endif
		g_RenderManager->setViewport(width, height);

	}

	void test::Clear()
	{
		//g_RenderManager->getShader().Use();
#if defined(DX11)

		/*std::vector<RenderTargetView> rtvs;
		rtvs.push_back(g_pRenderTargetView);
		rtvs.push_back(RTV2);

		g_RenderManager->ClearAndSetRenderTargets(rtvs, g_pDepthStencilView, g_ClearColor);*/
		g_RenderManager->getShader().SetPassOutputTexture("Lights", "Final", &g_pRenderTargetView, g_pDepthStencilView);
		//g_RenderManager->getShader().SetPassOutputTexture("GBuffer", "Final", &g_pRenderTargetView, g_pDepthStencilView);
#elif defined(OGL)
		glClearColor(g_ClearColor[0] * g_ClearColor[3], g_ClearColor[1] * g_ClearColor[3], g_ClearColor[2] * g_ClearColor[3], g_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	}

	void test::Display()
	{
#if defined(DX11)
		g_RenderManager->Present(0, 0);
#elif defined(OGL)
		glfwSwapBuffers(g_RenderManager->GetWindow());
#endif
	}

	void test::CleanupDevice()
	{
#if defined(DX11)

		g_pSamplerLinear.Release();

		g_RenderManager->ClearState();

		g_pRenderTargetView.Release();
		g_pDepthStencilView.Release();


		g_RenderManager->Release();

#endif
		delete g_RenderManager;
	}

	void test::Resize(unsigned int width, unsigned int height)
	{

#if defined(DX11)
		/*Release back buffer and depth stencil*/
		g_pRenderTargetView.Release();
		g_pDepthStencilView.Release();

		/*Resize the swap chain*/
		g_RenderManager->ResizeBuffers(MAX_SWAP_CHAIN_BUFFERS, width, height, FORMAT_R8G8B8A8_UNORM, 0);

		/*Recreate back buffer*/
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



		/*Recreate depth stencil*/
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
#endif
	}

  extern test& GetTestObj()
  {
	  static test* pTest = nullptr;
	  if (nullptr == pTest)
	  {
		  pTest = new test();
		  pTest->InitDevice();
	  }
	  return *pTest;
  }
}