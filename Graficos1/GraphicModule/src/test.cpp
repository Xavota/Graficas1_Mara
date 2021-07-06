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
#elif defined(OGL)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	int width = 0, height = 0;
	glfwGetFramebufferSize(g_RenderManager->GetWindow(), &width, &height);

	g_pRenderTargetView.Init(0, 0);
	g_pDepthStencilView.Init(width, height);

	//g_RenderManager->CompileShaders("miVertex.fx", "miPixel.fx");
#endif


	float color[4] = { 0.0f,0.0f,0.0f,1.0f };
	float SSAOcolor[4] = { 1.0f,1.0f,1.0f,1.0f };

#if defined(DX11)
		g_RenderManager->AddEffect("Deferred");
		g_RenderManager->AddEffect("Forward");/**/

		g_RenderManager->getShader("Deferred").CreatePass("GBuffer", "GBufferVertex.fx", "GBufferPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Deferred").CreatePass("SkyBox", "SkyBoxVertex.fx", "SkyBoxPixel.fx", CULL_BACK);
		g_RenderManager->getShader("Deferred").CreatePass("Lights", "LightVertex.fx", "LightPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Deferred").CreatePass("SSAO", "SSAOVertex.fx", "SSAOPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Deferred").CreatePass("ToneMap", "ToneMapVertex.fx", "ToneMapPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Deferred").CreatePass("Copy", "CopyVertex.fx", "CopyPixel.fx", CULL_FRONT);/**/


		g_RenderManager->getShader("Forward").CreatePass("SkyBox", "SkyBoxFwdVertex.fx", "SkyBoxFwdPixel.fx", CULL_BACK);
		g_RenderManager->getShader("Forward").CreatePass("Lights", "LightFwdVertex.fx", "LightFwdPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Forward").CreatePass("ToneMap", "ToneMapFwdVertex.fx", "ToneMapFwdPixel.fx", CULL_FRONT);
		g_RenderManager->getShader("Forward").CreatePass("Copy", "CopyFwdVertex.fx", "CopyFwdPixel.fx", CULL_FRONT);/**/
#elif defined(OGL)
	g_RenderManager->AddEffect("Deferred");
	g_RenderManager->AddEffect("Forward");/**/

	g_RenderManager->getShader("Deferred").CreatePass("GBuffer", "GBufferVertex.fx", "GBufferPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Deferred").CreatePass("SkyBox", "SkyBoxVertex.fx", "SkyBoxPixel.fx", CULL_BACK);
	g_RenderManager->getShader("Deferred").CreatePass("Lights", "LightVertex.fx", "LightPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Deferred").CreatePass("SSAO", "SSAOVertex.fx", "SSAOPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Deferred").CreatePass("ToneMap", "ToneMapVertex.fx", "ToneMapPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Deferred").CreatePass("Copy", "CopyVertex.fx", "CopyPixel.fx", CULL_FRONT);/**/



	g_RenderManager->getShader("Forward").CreatePass("SkyBox", "SkyBoxFwdVertex.fx", "SkyBoxFwdPixel.fx", CULL_BACK);
	g_RenderManager->getShader("Forward").CreatePass("Lights", "LightFwdVertex.fx", "LightFwdPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Forward").CreatePass("ToneMap", "ToneMapFwdVertex.fx", "ToneMapFwdPixel.fx", CULL_FRONT);
	g_RenderManager->getShader("Forward").CreatePass("Copy", "CopyFwdVertex.fx", "CopyFwdPixel.fx", CULL_FRONT);/**/
#endif

#if defined(DX11)
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("ViewMatrix", 0, sizeof(ViewMat));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("ProjectionMatrix", 1, sizeof(ProjectionMat));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("ModelMatrix", 2, sizeof(ModelMat));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("ViewPosition", 3, sizeof(Vector4));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("Material", 4, sizeof(Material));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("DirectionalLight", 5, sizeof(DirectionalLight));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("PointLight", 6, sizeof(PointLight));
		g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight", 7, sizeof(SpotLight));

		g_RenderManager->getShader("Deferred").AddPassTrackValue("GBuffer", "Bones", 8, sizeof(MATRIX) * 100);

		g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO", 0, sizeof(SSAO));

		g_RenderManager->getShader("Deferred").AddPassTrackValue("ToneMap", "Exposure", 0, sizeof(Vector4));/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "diffuse");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "normal");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "specular");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Position", true, color);
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Color", true, color);
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Normal", true, color);
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Specular", true, color);/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("SkyBox", "diffuse");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("SkyBox", "Final", true, color);/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "position");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "diffuse");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "normal");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "specular");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("Lights", "Final", false, color);/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("SSAO", "normal");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("SSAO", "position");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("SSAO", "Final", true, SSAOcolor);/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("ToneMap", "Light");
		g_RenderManager->getShader("Deferred").AddPassInputTexture("ToneMap", "SSAO");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("ToneMap", "Final", true, color);/**/

		g_RenderManager->getShader("Deferred").AddPassInputTexture("Copy", "Input");
		g_RenderManager->getShader("Deferred").AddPassOutputTexture("Copy", "Final", true, color);/**/

		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("SkyBox", "Final", "ToneMap", "Light");

		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Position", "Lights", "position");
		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Color", "Lights", "diffuse");
		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Normal", "Lights", "normal");
		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Specular", "Lights", "specular");/**/

		g_RenderManager->getShader("Deferred").UniteOutputOutputTextures("SkyBox", "Final", "Lights", "Final");/**/

		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Normal", "SSAO", "normal");
		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Position", "SSAO", "position");/**/
		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("SSAO", "Final", "ToneMap", "SSAO");/**/

		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("ToneMap", "Final", "Copy", "Input");/*

		g_RenderManager->getShader("Deferred").UniteInputOutputTextures("SSAO", "Final", "Copy", "Input");/**/

		g_RenderManager->getShader("Deferred").SetPassOutputTexture("Copy", "Final", &g_pRenderTargetView, g_pDepthStencilView);/*
		g_RenderManager->getShader("Deferred").SetPassOutputTexture("SkyBox", "Final", &g_pRenderTargetView, g_pDepthStencilView);/*
		g_RenderManager->getShader("Deferred").SetPassOutputTexture("GBuffer", "Position", &g_pRenderTargetView, g_pDepthStencilView);/**/
#elif defined(OGL)
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("ViewMatrix", "view", eDataType::MAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("ProjectionMatrix", "projection", eDataType::MAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("ModelMatrix", "model", eDataType::MAT);

	g_RenderManager->getShader("Deferred").AddEffectTrackValue("ViewPosition", "view_pos", eDataType::FLOAT4);

	g_RenderManager->getShader("Deferred").AddEffectTrackValue("Material_ambient", "mat.ambient", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("Material_specular", "mat.specular", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("Material_diffuse", "mat.diffuse", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("Material_shininess", "mat.shininess", eDataType::FLOAT);

	g_RenderManager->getShader("Deferred").AddEffectTrackValue("DirectionalLight_dir", "dirL.lightDir", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("DirectionalLight_color", "dirL.color", eDataType::FLOAT4);

	g_RenderManager->getShader("Deferred").AddEffectTrackValue("PointLight_pos", "pointL.lightPos", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("PointLight_color", "pointL.lightColor", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("PointLight_att", "pointL.lightAtt", eDataType::FLOAT);

	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_pos", "spotL.lightPos", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_dir", "spotL.lightDir", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_color", "spotL.lightColor", eDataType::FLOAT4);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_att", "spotL.lightAtt", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_inner", "spotL.cutOff", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddEffectTrackValue("SpotLight_outer", "spotL.outerCutOff", eDataType::FLOAT);

	g_RenderManager->getShader("Deferred").AddPassTrackValue("GBuffer", "Bones", "bones", eDataType::MAT, 100);

	g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO_rad", "_ssao.sampleRadius", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO_scale", "_ssao.scale", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO_bias", "_ssao.bias", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO_intens", "_ssao.intensity", eDataType::FLOAT);
	g_RenderManager->getShader("Deferred").AddPassTrackValue("SSAO", "SSAO_it", "_ssao.sampleIterations", eDataType::INT);

	g_RenderManager->getShader("Deferred").AddPassTrackValue("ToneMap", "Exposure", "exposure", eDataType::FLOAT);/**/



	g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "diffuse", "diffuseMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "normal", "normalMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("GBuffer", "specular", "specularMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Position", true, color);
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Color", true, color);
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Normal", true, color);
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Specular", true, color);
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("GBuffer", "Mask", true, color);/**/


	g_RenderManager->getShader("Deferred").AddPassInputTexture("SkyBox", "diffuse", "skyBoxMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("SkyBox", "Final", true, color);/**/


	g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "position", "positionMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "diffuse", "colorMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "normal", "normalMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "specular", "specularMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("Lights", "mask", "maskMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("Lights", "Final", false, color);/**/


	g_RenderManager->getShader("Deferred").AddPassInputTexture("SSAO", "normal", "normalMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("SSAO", "position", "positionMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("SSAO", "mask", "maskMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("SSAO", "Final", true, SSAOcolor);/**/


	g_RenderManager->getShader("Deferred").AddPassInputTexture("ToneMap", "Light", "LightMap");
	g_RenderManager->getShader("Deferred").AddPassInputTexture("ToneMap", "SSAO", "SSAOMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("ToneMap", "Final", true, color);/**/


	g_RenderManager->getShader("Deferred").AddPassInputTexture("Copy", "Input", "inputMap");
	g_RenderManager->getShader("Deferred").AddPassOutputTexture("Copy", "Final", true, color);/**/




	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Position", "Lights", "position");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Color", "Lights", "diffuse");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Normal", "Lights", "normal");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Specular", "Lights", "specular");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Mask", "Lights", "mask");/**/

	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Normal", "SSAO", "normal");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Position", "SSAO", "position");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("GBuffer", "Mask", "SSAO", "mask");
	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("SSAO", "Final", "ToneMap", "SSAO");/**/

	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("SkyBox", "Final", "ToneMap", "Light");
	
	//g_RenderManager->getShader("Deferred").UniteOutputOutputTextures("SkyBox", "Final", "Lights", "Final");/**/

	g_RenderManager->getShader("Deferred").UniteInputOutputTextures("ToneMap", "Final", "Copy", "Input");

	g_RenderManager->getShader("Deferred").SetPassOutputTexture("Copy", "Final", &g_pRenderTargetView, g_pDepthStencilView);
#endif

		/*Forward*/
#if defined(DX11)
		g_RenderManager->getShader("Forward").AddEffectTrackValue("ViewMatrix", 0, sizeof(ViewMat));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("ProjectionMatrix", 1, sizeof(ProjectionMat));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("ModelMatrix", 2, sizeof(ModelMat));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("ViewPosition", 3, sizeof(Vector4));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("Material", 4, sizeof(Material));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("DirectionalLight", 5, sizeof(DirectionalLight));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("PointLight", 6, sizeof(PointLight));
		g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight", 7, sizeof(SpotLight));/**/

		g_RenderManager->getShader("Forward").AddPassTrackValue("ToneMap", "Exposure", 0, sizeof(Vector4));/**/



		g_RenderManager->getShader("Forward").AddPassInputTexture("SkyBox", "diffuse");
		g_RenderManager->getShader("Forward").AddPassOutputTexture("SkyBox", "Final", true, color);/**/


		g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "diffuse");
		g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "AO");
		g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "normal");
		g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "specular");
		g_RenderManager->getShader("Forward").AddPassOutputTexture("Lights", "Final", false, color);/**/


		g_RenderManager->getShader("Forward").AddPassInputTexture("ToneMap", "Light");
		g_RenderManager->getShader("Forward").AddPassOutputTexture("ToneMap", "Final", true, color);/**/


		g_RenderManager->getShader("Forward").AddPassInputTexture("Copy", "Input");
		g_RenderManager->getShader("Forward").AddPassOutputTexture("Copy", "Final", true, color);/**/


		g_RenderManager->getShader("Forward").UniteInputOutputTextures("SkyBox", "Final", "ToneMap", "Light");
		g_RenderManager->getShader("Forward").UniteInputOutputTextures("ToneMap", "Final", "Copy", "Input");

		g_RenderManager->getShader("Forward").UniteOutputOutputTextures("SkyBox", "Final", "Lights", "Final");/**/

		g_RenderManager->getShader("Forward").SetPassOutputTexture("Copy", "Final", &g_pRenderTargetView, g_pDepthStencilView);/*
		g_RenderManager->getShader("Forward").SetPassOutputTexture("SkyBox", "Final", &g_pRenderTargetView, g_pDepthStencilView);/**/
#elif defined(OGL)
	g_RenderManager->getShader("Forward").AddEffectTrackValue("ViewMatrix", "view", eDataType::MAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("ProjectionMatrix", "projection", eDataType::MAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("ModelMatrix", "model", eDataType::MAT);

	g_RenderManager->getShader("Forward").AddEffectTrackValue("ViewPosition", "view_pos", eDataType::FLOAT4);

	g_RenderManager->getShader("Forward").AddEffectTrackValue("Material_ambient", "mat.ambient", eDataType::FLOAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("Material_specular", "mat.specular", eDataType::FLOAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("Material_diffuse", "mat.diffuse", eDataType::FLOAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("Material_shininess", "mat.shininess", eDataType::FLOAT);

	g_RenderManager->getShader("Forward").AddEffectTrackValue("DirectionalLight_dir", "dirL.lightDir", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("DirectionalLight_color", "dirL.color", eDataType::FLOAT4);

	g_RenderManager->getShader("Forward").AddEffectTrackValue("PointLight_pos", "pointL.lightPos", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("PointLight_color", "pointL.lightColor", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("PointLight_att", "pointL.lightAtt", eDataType::FLOAT);

	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_pos", "spotL.lightPos", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_dir", "spotL.lightDir", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_color", "spotL.lightColor", eDataType::FLOAT4);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_att", "spotL.lightAtt", eDataType::FLOAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_inner", "spotL.cutOff", eDataType::FLOAT);
	g_RenderManager->getShader("Forward").AddEffectTrackValue("SpotLight_outer", "spotL.outerCutOff", eDataType::FLOAT);

	g_RenderManager->getShader("Forward").AddPassTrackValue("ToneMap", "Exposure", "exposure", eDataType::FLOAT);/**/



	g_RenderManager->getShader("Forward").AddPassInputTexture("SkyBox", "diffuse", "skyBoxMap");
	g_RenderManager->getShader("Forward").AddPassOutputTexture("SkyBox", "Final", false, color);/**/


	g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "diffuse", "diffuseMap");
	g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "AO", "AOMap");
	g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "normal", "normalMap");
	g_RenderManager->getShader("Forward").AddPassInputTexture("Lights", "specular", "specularMap");
	g_RenderManager->getShader("Forward").AddPassOutputTexture("Lights", "Final", true, color);/**/


	g_RenderManager->getShader("Forward").AddPassInputTexture("ToneMap", "Light", "LightMap");
	g_RenderManager->getShader("Forward").AddPassOutputTexture("ToneMap", "Final", true, color);/**/


	g_RenderManager->getShader("Forward").AddPassInputTexture("Copy", "Input", "inputMap");
	g_RenderManager->getShader("Forward").AddPassOutputTexture("Copy", "Final", true, color);/**/



	g_RenderManager->getShader("Forward").UniteInputOutputTextures("SkyBox", "Final", "ToneMap", "Light");
	g_RenderManager->getShader("Forward").UniteInputOutputTextures("ToneMap", "Final", "Copy", "Input");


	g_RenderManager->getShader("Forward").SetPassOutputTexture("Copy", "Final", &g_pRenderTargetView, g_pDepthStencilView);/*
	g_RenderManager->getShader("Forward").SetPassOutputTexture("Lights", "Final", &g_pRenderTargetView, g_pDepthStencilView);/**/
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

#elif defined(OGL)
		//glClearColor(0, 0, 0,1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
		g_RenderManager->getShader(m_Technique).Draw();/*
		g_RenderManager->getShader("Forward").Draw();/**/
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