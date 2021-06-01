#pragma once
#include "Shader.h"
#include "OBJInstance.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

#include <map>

namespace GraphicsModule
{
	enum class eDataType
	{
		NONE = -1,
		BOOL,
		INT,
		FLOAT,
		UINT,
		BOOL2,
		INT2,
		FLOAT2,
		UNIT2,
		BOOL3,
		INT3,
		FLOAT3,
		UINT3,
		BOOL4,
		INT4,
		FLOAT4,
		UINT4
	};

	struct Values
	{
#if defined(DX11)
		Values(string name, unsigned int id, unsigned int size);
#elif defined(OGL)
		Values(string name, string uniform, eDataType type);
#endif
		Values(const Values& other);
		~Values();
		string m_name;
#if defined(DX11)
		unsigned int m_id = 0;
		Buffer m_buff;
		unsigned int m_size = 0;
#elif defined(OGL)
		string m_uniform;
		eDataType m_type;
#endif
		void* m_data = nullptr;
	};

class Pass
{
public:
	Pass() = default;
	~Pass() = default;

	void DrawPass();

	void Compile(const char* vertexShaderString, const char* pixelShaderString);

	void Use();
	void Draw(unsigned int indexCount);

#if defined(DX11)
	void SetBuffer(int slot, Buffer buff, void* data);

#elif defined(OGL)
	void Unuse();
	void SetBool(const string name, bool value);
	void SetInt(const string name, int value);
	void SetFloat(const string name, float value);
	void SetUint(const string name, unsigned int value);

	void SetBool2(const string name, bool value1, bool value2);
	void SetInt2(const string name, int value1, int value2);
	void SetFloat2(const string name, float value1, float value2);
	void SetUint2(const string name, unsigned int value1, unsigned int value2);

	void SetBool3(const string name, bool value1, bool value2, bool value3);
	void SetInt3(const string name, int value1, int value2, int value3);
	void SetFloat3(const string name, float value1, float value2, float value3);
	void SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3);

	void SetBool4(const string name, bool value1, bool value2, bool value3, bool value4);
	void SetInt4(const string name, int value1, int value2, int value3, int value4);
	void SetFloat4(const string name, float value1, float value2, float value3, float value4);
	void SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4);

	void SetMat4(const string name, glm::mat4 value);

	void SetInputLayout(unsigned int VAO);
#endif



#if defined(DX11)
	void AddTrackValue(string name, unsigned int id, unsigned int size);
#elif defined(OGL)
	void AddTrackValue(string name, string uniform, eDataType type);
#endif
	
	void AddInputTexture(string name);
	void SetInputTexture(string name, Texture tex);
	void AddOutputTexture(string name);
	void SetOutputTexture(string name, RenderTargetView* tex, DepthStencilView dsv);

	void SetValue(string name, void* data);


private:
	struct OutputTexture
	{
		string m_name;
		RenderTargetView* m_renderTarget;
		DepthStencilView m_depthStencil;
	};
	Shader m_shaders;
	std::vector<Values> m_values;

	struct InputTexture
	{
		string m_name;
		Texture m_texture;
	};

	std::vector<InputTexture> m_inputTextures;
	std::vector<OutputTexture> m_outputTextures;
};
}
