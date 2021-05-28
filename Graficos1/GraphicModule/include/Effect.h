#pragma once
#include "Technique.h"
#include <vector>

#define TEXTURE_MAP_DIFFUSE 1
#define TEXTURE_MAP_NORMAL 2
#define TEXTURE_MAP_SPECULAR 4

#define TEXTURE_MAP_COUNT (TEXTURE_MAP_DIFFUSE | TEXTURE_MAP_NORMAL | TEXTURE_MAP_SPECULAR)

enum class eNORMAL_TECHNIQUES
{
	NONE = -1,
	VERTEX_SHADER,
	PIXEL_SHADER,
	COUNT
};

enum class eSPECULAR_TECHNIQUES
{
	NONE = -1,
	PHONG,
	BLINN_PHONG,
	COUNT
};

namespace GraphicsModule
{
class Effect
{
public:
	Effect() = default;
	~Effect() = default;

	void CompileShader(const char* vertexShaderPath, const char* pixelShaderPath);

	void SetShaderFlags(eNORMAL_TECHNIQUES nor, eSPECULAR_TECHNIQUES spec, unsigned int texFlags);

	void Use(); 

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

private:
	struct Techs
	{
		eNORMAL_TECHNIQUES normalTech;
		eSPECULAR_TECHNIQUES specularTech;
		unsigned int texFlagsTech;
		Technique tech;
	};
	std::vector<Techs> m_techniques;
	Technique* m_currentTechnique;
};
}
