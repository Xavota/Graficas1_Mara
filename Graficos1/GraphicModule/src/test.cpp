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

		if (g_RenderManager->CreateDevices(width, height, driverTypes, numDriverTypes, createDeviceFlags, featureLevels, numFeatureLevels, &g_featureLevel))
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

		g_RenderManager->OMSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView);



		/*Compile vertex and pixel shaders*/
		g_RenderManager->CompileShaders("Tutorial07.fx", "VS", "vs_4_0", "Tutorial07.fx", "PS", "ps_4_0");


		g_RenderManager->IASetInputLayout(g_RenderManager->GetInputLayout());


		/*Create buffers that pass data to the shaders*/
		BUFFER_DESC bd;
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
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(Vector4);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetViewPositionBuffer());/**/
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(Material);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetMaterialShininessBuffer());/**/
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(DirectionalLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetDirectionalLightBuffer());/**/
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(PointLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetPointLightBuffer());/**/
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(SpotLight);
		hr = g_RenderManager->CreateBuffer(&bd, NULL, g_RenderManager->GetSpotLightBuffer());/**/
		if (FAILED(hr))
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
#elif defined(OGL)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	g_RenderManager->CompileShaders("miVertex.fx", "miPixel.fx");
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
#if defined(DX11)
		g_RenderManager->VSSetShader(g_RenderManager->GetVertexShader(), NULL, 0);
		g_RenderManager->PSSetShader(g_RenderManager->GetPixelShader(), NULL, 0);

		g_RenderManager->ClearAndSetRenderTargets(1, g_pRenderTargetView, g_pDepthStencilView, g_ClearColor);

#elif defined(OGL)
		g_RenderManager->getShader().Use();

		/*Clear screen*/
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