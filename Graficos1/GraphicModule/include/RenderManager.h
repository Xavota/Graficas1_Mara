#pragma once

#include <string>
using std::string;

#include "Descriptors.h"

#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"

#include "Texture2D.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "Buffer.h"

#include "VertexShader.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "SamplerState.h"

#include "RasterizeState.h"

#include "OBJInstance.h"

#if defined(DX11)
#include <d3dcompiler.h>
#elif defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif

//#include "Shader.h"
#include "Effect.h"


namespace GraphicsModule
{

	struct RenderTargetStruct
	{
		RenderTargetStruct(string name, RenderTargetView rtv, DepthStencilView dst, Texture tex);
		RenderTargetStruct(string name);
		RenderTargetStruct(const RenderTargetStruct& other);
		~RenderTargetStruct();
		std::vector<string> m_names;
		RenderTargetView m_rtv;
		DepthStencilView m_dsv;
		Texture m_tex;
	};

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

#if defined(DX11)
	inline ID3D11Device*& getDevicePtr() { return m_device.GetDevicePtr(); }
	inline ID3D11DeviceContext*& getDeviceContextPtr() { return m_deviceContext.getDeviceContextPtr(); }
	inline IDXGISwapChain*& getSwapChainPtr() { return m_swapChain.GetSwapChainPtr(); }

	/*Device functions*/
	HRESULT CreateRenderTargetView( Texture2D& texture, const RENDER_TARGET_VIEW_DESC* desc, RenderTargetView& rtv);
	HRESULT CreateTexture2D( const TEXTURE2D_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Texture2D& ppTexture2D);
	HRESULT CreateDepthStencilView( Texture2D& pResource, const DEPTH_STENCIL_VIEW_DESC* pDesc, DepthStencilView& ppDepthStencilView);
	HRESULT CreateVertexShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, VertexShader& ppVertexShader);
	HRESULT CreateInputLayout( const INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, const void* pShaderBytecodeWithInputSignature, 
							  SIZE_T BytecodeLength, InputLayout& ppInputLayout);
	HRESULT CreatePixelShader( const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		PixelShader& ppPixelShader);
	HRESULT CreateBuffer( const BUFFER_DESC* pDesc, const SUBRESOURCE_DATA* pInitialData, Buffer& ppBuffer);
	HRESULT CreateSamplerState(const SAMPLER_DESC* pSamplerDesc, SamplerState& ppSamplerState);
	HRESULT CreateRasterizerState( const RASTERIZER_DESC* pRasterizerDesc, RasterizeState& ppRasterizerState);
	HRESULT CreateShaderResourceView( Texture2D& pResource, const SHADER_RESOURCE_VIEW_DESC* pDesc, ShaderResourceView& ppSRView);

	/*Device context functions*/
	void UpdateSubresource( Buffer& pDstResource, unsigned int DstSubresource, const BOX* pDstBox, const void* pSrcData,
							unsigned int SrcRowPitch, unsigned int SrcDepthPitch );
	void UpdateTexture2D( Texture2D& image, const void* data, unsigned int rowSize);
	void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );
	void OMSetRenderTargets(unsigned int count, RenderTargetView* ppRenderTargetViews, DepthStencilView& pDepthStencilView);
	void ClearAndSetRenderTargets(unsigned int count, RenderTargetView* ppRenderTargetViews, DepthStencilView& pDepthStencilView, std::vector<float*> ColorRGBA, std::vector<bool> cleans);
	void RSSetViewports( unsigned int NumViewports, const VIEWPORT* pViewports );
	void IASetInputLayout( InputLayout& pInputLayout );
	void IASetVertexBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer& ppVertexBuffers, 
							 const unsigned int* pStrides, const unsigned int* pOffsets);
	void IASetIndexBuffer( Buffer& pIndexBuffer, FORMAT Format, unsigned int Offset);
	void IASetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology);
	void ClearRenderTargetView( RenderTargetView& pRenderTargetView, const float ColorRGBA[4]);
	void ClearDepthStencilView( DepthStencilView& pDepthStencilView, unsigned int ClearFlags, float Depth, unsigned char Stencil);
	void VSSetShader( VertexShader& pVertexShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void VSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShader(PixelShader pPixelShader, ID3D11ClassInstance* const* ppClassInstances, unsigned int NumClassInstances);
	void PSSetConstantBuffers( unsigned int StartSlot, unsigned int NumBuffers, Buffer ppConstantBuffers);
	void PSSetShaderResources( unsigned int StartSlot, std::vector<Texture> ppShaderResourceViews);
	void PSSetSamplers( unsigned int StartSlot, unsigned int NumSamplers, SamplerState& ppSamplers);
	void RSSetState( RasterizeState pRasterizerState);
	void Flush();
	void ClearState();

	/*Swap Chain Functions*/
	HRESULT GetBuffer( unsigned int Buffer, REFIID riid, void** ppSurface);
	HRESULT ResizeBuffers( unsigned int BufferCount, unsigned int Width, unsigned int Height, FORMAT NewFormat, unsigned int SwapChainFlags);
	//HRESULT Resize(unsigned int width, unsigned int height);
	void Present( unsigned int SyncInterval, unsigned int Flags);

	/*Globals*/
	HRESULT CreateDeviceAndSwapChain( IDXGIAdapter* pAdapter, DRIVER_TYPE DriverType, HMODULE Software,
		unsigned int Flags, const FEATURE_LEVEL* pFeatureLevels, unsigned int FeatureLevels, unsigned int SDKVersion,
		const SWAP_CHAIN_DESC* pSwapChainDesc, FEATURE_LEVEL* pFeatureLevel);

	HRESULT CreateShaderResourceViewFromFile( LPCSTR pSrcFile, D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
			ShaderResourceView& ppShaderResourceView, HRESULT* pHResult);

	HRESULT CreateShaderAsRenderTargetView(ShaderResourceView& ViewRT, RenderTargetView& RenderTargetView,
		unsigned int width, unsigned int height);

	HRESULT CreateDevices(unsigned int width, unsigned int height,
						  const DRIVER_TYPE* driverTypes, unsigned int numDriverTypes, unsigned int Flags,
						  const FEATURE_LEVEL* pFeatureLevels, unsigned int numFeatureLevels, FEATURE_LEVEL* pFeatureLevel);

	HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CompileShaderFromString(const char* source, unsigned int bytesCount, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, InputLayout& pInputLayout);

	//void SetBuffer(int slot, Buffer buff, void* data);

#elif defined(OGL)

	void ShaderSetBool(const string name, bool value);
	void ShaderSetInt(const string name, int value);
	void ShaderSetFloat(const string name, float value);
	void ShaderSetUint(const string name, unsigned int value);

	void ShaderSetBool2(const string name, bool value1, bool value2);
	void ShaderSetInt2(const string name, int value1, int value2);
	void ShaderSetFloat2(const string name, float value1, float value2);
	void ShaderSetUint2(const string name, unsigned int value1, unsigned int value2);

	void ShaderSetBool3(const string name, bool value1, bool value2, bool value3);
	void ShaderSetInt3(const string name, int value1, int value2, int value3);
	void ShaderSetFloat3(const string name, float value1, float value2, float value3);
	void ShaderSetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3);

	void ShaderSetBool4(const string name, bool value1, bool value2, bool value3, bool value4);
	void ShaderSetInt4(const string name, int value1, int value2, int value3, int value4);
	void ShaderSetFloat4(const string name, float value1, float value2, float value3, float value4);
	void ShaderSetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4);/**/

	void ShaderSetMat4(const string name, std::vector<glm::mat4> value);

#endif
	//Shader& getShader() { return m_shader; }
	void AddEffect(string name);
	Effect& getShader(string name);

	HRESULT CompileShaders(const char* vsFileName, const char* psFileName);
	void SetShaderFlags(string EffectName, eNORMAL_TECHNIQUES nor, eSPECULAR_TECHNIQUES spec, unsigned int texFlags, eTONE_CORRECTION_TECHNIQUES toneMap/**/);

	void setViewport(unsigned int width, unsigned int height);

	void UpdateViewMatrix(MATRIX view);
	void UpdateProjectionMatrix(MATRIX projection);
	void UpdateModelMatrix(MATRIX model);

	void UpdateMaterial(Material matDesc);
	void UpdateDirectionalLight(DirectionalLight dirDesc);
	void UpdatePointLight(PointLight pointDesc);
	void UpdateSpotLight(SpotLight spotDesc);

#if defined(DX11)
	Buffer& GetNeverChangesBuffer() { return m_pCBNeverChanges; }
	Buffer& GetChangeOnResizeBuffer() { return m_pCBChangeOnResize; }
	Buffer& GetChangesEveryFrameBuffer() { return m_pCBChangesEveryFrame; }

	Buffer& GetViewPositionBuffer() { return m_ViewPositionBuffer; }
	Buffer& GetMaterialShininessBuffer() { return m_MaterialShininessBuffer; }

	Buffer& GetDirectionalLightBuffer() { return m_DirectionalLightBuffer; }
	Buffer& GetPointLightBuffer() { return m_PointLightBuffer; }
	Buffer& GetSpotLightBuffer() { return m_SpotLightBuffer; }
#endif	
	bool RenderTargetExist(string name);
	bool AddRenderTargetAndTexture(string name);
	void AddRedefinitionOfRenderTarget(string name, string newName);
	std::vector<RenderTargetStruct>* GetRenderTargets();
	RenderTargetView* GetRenderTarget(string name);
	DepthStencilView* GetDepthStencil(string name);
	Texture* GetTexture(string name);

#if !defined(OGL)
	HWND& GetWindow() { return m_hwnd; }
	void SetWindow(HWND window) { m_hwnd = window; }
#else
	GLFWwindow*& GetWindow() { return m_window; }
	void SetWindow(GLFWwindow* window) { m_window = window; }
#endif
	/*Clear*/
	void Release();


private:
#if defined(DX11)
	Device									m_device;
	DeviceContext							m_deviceContext;
	SwapChain								m_swapChain;
#endif
#if !defined(OGL)
	HWND m_hwnd;
#else
	GLFWwindow* m_window;
#endif

	struct EffectStruct
	{
		string m_name;
		Effect m_effect;
	};

	std::vector<EffectStruct> m_effects;

	//Shader m_shader;

	std::vector<RenderTargetStruct> m_rtvs;

public:
#if defined(DX11)
	Buffer									m_pCBNeverChanges;
	Buffer									m_pCBChangeOnResize;
	Buffer									m_pCBChangesEveryFrame;

	Buffer									m_ViewPositionBuffer;
	Buffer									m_MaterialShininessBuffer;

	Buffer									m_DirectionalLightBuffer;
	Buffer									m_PointLightBuffer;
	Buffer									m_SpotLightBuffer;
#endif
};
extern RenderManager* GetManager();

}